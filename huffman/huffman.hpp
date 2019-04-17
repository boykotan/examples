#include <string>
#include <map>

struct Node{
    size_t get_frec() const;
    uint8_t get_letter() const;
    void gen_code(std::string code, std::map<char, std::string> &alphabet);
    bool operator() (const Node& x, const Node& y) const;
    Node(Node* nleft, Node* nright);
    Node(size_t fr, Node* nleft, Node* nright);
    Node(size_t fr, char lt);
  private:
    Node *right_;
    Node *left_;
    char letter_;
    size_t freq_;
};