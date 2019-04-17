#include <string>
#include <map>
#include <iostream>
#include "huffman.hpp"

size_t Node::get_frec() const {
    return freq_;
}

uint8_t Node::get_letter() const {
    return letter_*1;
}

bool Node::operator() (const Node& x, const Node& y) const {
    return x.get_frec() > y.get_frec();
}

void Node::gen_code(std::string code, std::map<char, std::string> &alphabet) {
    if (left_ || right_) {
		if (left_) 
			left_->gen_code(code + "1", alphabet);
		if (right_)
			right_->gen_code(code + "0", alphabet);
		if (left_) 
			delete left_;
		if (right_)
			delete right_;
    } else
        alphabet[letter_] = code;
}

Node::Node(Node* nleft, Node* nright) : right_(nright), left_(nleft), letter_(),
                    freq_(nleft->get_frec() + nright->get_frec()) {}
					
					
Node::Node(size_t fr, Node* nleft, Node* nright) : right_(nright), left_(nleft),
                    letter_(), freq_(fr) {}

Node::Node(size_t fr, char lt) : right_(nullptr), left_(nullptr), letter_(lt), freq_(fr) {}
