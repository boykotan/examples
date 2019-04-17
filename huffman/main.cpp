#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <map>
#include <queue>
#include <cstdlib>
#include "huffman.hpp"
#include <iomanip>
#include <climits>

// for tree rearrange
bool P_Compare(const Node* a, const Node* b){
    return a->get_frec() > b->get_frec();
}

// check byte overflow
int catch_pcodebuf(uint8_t &bit_pos, char &buf, std::fstream &file, int len = -1) {
    if (bit_pos == 8) {
        bit_pos = 0;
		if (len > 0) {
			file.get(buf);
			len--;
		}
		else
			file.put(buf);
    }
    return len;
}

// create dictionary during encode
// return haffman tree size (alfabet letter count)
int encode_dict(std::map<char,std::string> &alphabet, std::fstream &file_r, uint32_t &indata) {
    char buf;
    file_r.get(buf);
    uint32_t frec_letter[256] = {0};
    while (file_r.gcount()) {			
            indata++;
            frec_letter[static_cast<unsigned char>(buf)]++;
            file_r.get(buf);
    }

    std::priority_queue<Node*, std::vector<Node*>, bool (*)(const Node*, const Node*)> p_graph(P_Compare);

    for (size_t i = 0; i < 256; i++)
        if (frec_letter[i] > 0){
			Node *p_node=new Node(frec_letter[i], /* (char)i */static_cast<char>(i));
			p_graph.push(p_node);
		}
		
	size_t alpha_count = p_graph.size();

    while (p_graph.size() > 1) {
            Node *n1 = p_graph.top();p_graph.pop();
            Node *n2 = p_graph.top();p_graph.pop();
			Node* combined = new Node(n1,n2);
            p_graph.push(combined);
	}
	if (!p_graph.empty()){
		Node *result = p_graph.top();
		if (alpha_count > 1)
			result->gen_code("", alphabet);
		else
			result->gen_code("1", alphabet);
		delete result;
	}
    return alpha_count;
}


//encode file_in and write result to file_out
int encode(const char* file_in, const char* file_out, bool dict_print){

    std::fstream file_r(file_in, std::ios::in | std::ios::binary);
    if (!file_r)
		return EXIT_FAILURE;
    char buf=0;
    std::map<char,std::string> alphabet;
    uint32_t indata = 0, outdata = 0;
    size_t alpha_count = encode_dict(alphabet, file_r, indata);	//make dict
    std::fstream file_w(file_out, std::ios_base::out | std::ios_base::binary);
    if (!file_w)
        return EXIT_FAILURE;
	uint32_t tree_size = 0;
	for (auto i : alphabet)   //calculate dict size
		tree_size += i.second.length();
	
	tree_size += alpha_count*2*8;
	file_w.write(reinterpret_cast<const char *>(&tree_size), sizeof(uint32_t));

	uint8_t bit_index = 0;
	for (auto i : alphabet) {   //write dict
		int help = 128;
		for (size_t j = 0; j < 8; j++) {
			buf = buf << 1;
			if (i.first & help)
				buf = buf | 1;
			bit_index++;
			help = help >> 1;
			catch_pcodebuf(bit_index, buf, file_w);
		}		
		help = 128;
		for (size_t j = 0; j < 8; j++) {
			buf = buf << 1;
			if (i.second.length() & help)
				buf = buf | 1;
			bit_index++;
			help = help >> 1;
			catch_pcodebuf(bit_index, buf, file_w);
		}

		for (auto j : i.second) {
			buf = buf << 1;
			if (j == '1')
				buf = buf | 1;
			bit_index++;
			catch_pcodebuf(bit_index, buf, file_w);
		}
	}
	if (bit_index) { // check last bits
		buf = buf << (8 - bit_index);
		file_w.put(buf);
	}
	file_w.write(reinterpret_cast<const char *>(&indata), sizeof(uint32_t));
	bit_index = 0;
	
	file_r.clear();
	file_r.seekg(0);
	char outbuf=0;
	file_r.get(buf);
	while (file_r.gcount()) {		//encode input file
		for (size_t j = 0; j < alphabet[buf].length(); j++) {
			outbuf = outbuf << 1;
			if (alphabet[buf][j] == '1')
				outbuf = outbuf | 1;
			bit_index++;
			if (bit_index == 8) {
				bit_index = 0;
				file_w.put(outbuf);
				outdata++;
			}
		}
		file_r.get(buf);
	}
	if (bit_index) {
		outbuf = outbuf << (8 - bit_index);
		file_w.put(outbuf);
		outdata++;
	}

	std::cout << indata << std::endl << outdata << std::endl << ceil(tree_size/8.0) + 2 * sizeof(uint32_t) << std::endl;	//statistic output
	if (dict_print) {
		std::map<std::string, char> alphabet_print;
		for (auto i:alphabet)
			alphabet_print[i.second] = i.first;
		for (auto i:alphabet_print)
			std::cout << i.first << " " << static_cast<int>(static_cast<unsigned char>(i.second)) << std::endl;
	}
    return EXIT_SUCCESS;
}

int decode_dict_load(std::fstream &file_r, std::map<std::string,char> &alphabet, uint32_t &helpinfo) {
	uint32_t len1, inf = 0;
	uint8_t bit_index = 8;
	file_r.read(reinterpret_cast<char *>(&inf), sizeof(inf));
	len1 = inf/8;
	if (len1*8 < inf) len1++;
	helpinfo = len1 + 2*sizeof(uint32_t);
	char currcode = 0;
	while (len1 > 0){
		char alf_ch=0;
		len1 = catch_pcodebuf(bit_index, currcode, file_r, len1);
		//len1 = make_byte_symbol_get(currcode, file_r, alf_ch, bit_index, len1);
		//int    make_byte_symbol_get(char &f, std::fstream &file, char &buf, int &pcodebuf, int len = -1) 
		{
			int help = 128;
			for (int j = 0; j < 8; j++) {
				alf_ch = alf_ch << 1;
				if (currcode & help)
					alf_ch = alf_ch | 1;
				bit_index++;
				currcode = currcode << 1;
				len1 = catch_pcodebuf(bit_index, currcode, file_r, len1);
			}
		}
		size_t alf_len=0;
		int help = 128;
		for (size_t j = 0; j < 8; j++) {
			if (bit_index >= 8) {
				bit_index = 0;
				file_r.get(currcode);
				len1--;
			}
			alf_len = alf_len << 1;
			if (currcode & help)
				alf_len = alf_len | 1;
			currcode = currcode << 1;
			bit_index++;
		}
		
		std::string alf_code="";
		for (size_t j = 0; j < alf_len; j++) {
			len1 = catch_pcodebuf(bit_index, currcode, file_r, len1);
			alf_code += (currcode & help) ? "1" : "0";
			currcode = currcode << 1;
			bit_index++;
	   }
		alphabet[alf_code] = alf_ch;
	}
	return EXIT_SUCCESS;
}

int decode(const char* file_in, const char* file_out, bool dict_print){
//    using namespace std;
	size_t indata = 0;
	std::fstream file_r(file_in, std::ios::in | std::ios::binary);
	if (!file_r)
		return EXIT_FAILURE;
	uint32_t helpinfo;
	char buf;
	std::map<std::string,char> alphabet;
	if (decode_dict_load(file_r, alphabet, helpinfo))
		return EXIT_FAILURE;
	uint32_t inf=0;
	file_r.read(reinterpret_cast<char *>(&inf), sizeof(uint32_t));       
	size_t outdata = inf;
	//std::cout << "öutdata:" << outdata << std::endl;
	std::fstream file_w(file_out, std::ios::out | std::ios::binary);
	if (!file_w)
		return EXIT_FAILURE;
	
	std::string code = "";
	uint8_t bit_index=8;
	uint8_t help = 128;
	while (inf>0) {
		if (bit_index==8){
			file_r.get(buf);
			indata++;
			bit_index=0;
		}
		code += (buf & help) ? "1" : "0";
		buf = buf << 1;
		bit_index++;
		
		if (alphabet.count(code) > 0) {
			file_w.put(alphabet[code]);
			inf--;
			code = "";
		}
	}	
	
	std::cout << indata << std::endl << outdata << std::endl << helpinfo << std::endl;
	if (dict_print)
		for (auto i:alphabet)
//            cout << i.first << " " << static_cast<int>(i.second) << endl;
            std::cout << i.first << " " << static_cast<int>(static_cast<unsigned char>(i.second)) << std::endl;
	return EXIT_SUCCESS;
}
	
int main(int argc, char* argv[]) {
    if (argc < 4 || argc > 5) {
		std::cerr << "Error. Bad arg num";
        return EXIT_FAILURE;
	}
	else{

		if (std::string(argv[argc - 3]) == "-c") {
			if (encode(argv[argc - 2], argv[argc - 1],std::string(argv[argc - 4]) == "-v")) {
				std::cerr << "Error. File error";
				return EXIT_FAILURE;
			}
		}
		else if (std::string(argv[argc - 3]) == "-d") {
			if (decode(argv[argc - 2], argv[argc - 1],std::string(argv[argc - 4]) == "-v")) {
				std::cerr << "Error. File error";
				return EXIT_FAILURE;
			}
		}
		else {
			std::cerr << "Error. (en|de)code not found";
			return EXIT_FAILURE;
		}
	}
    return EXIT_SUCCESS;
}
