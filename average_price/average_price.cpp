
#include <iostream>
#include <ranges>
#include <string>
#include <string_view>
#include <unordered_map>

#include <boost/algorithm/string.hpp>


using namespace std;
using namespace boost;



struct fill{
  fill(vector<string> tokens)
    : rootOrderid(0),
      orderid(stoi(tokens[1])),
      qty(stoi(tokens[4])),
      price(stod(tokens[5])) {}

  int rootOrderid;
  int orderid;
  int qty;
  double price;
};

struct order{
  order(vector<string> tokens)
      : orderid(stoi(tokens[1])),
        qty(stoi(tokens[4])),
        price(stod(tokens[5]))
  {}

  int orderid;
  int qty;
  double price;
};

struct newOrder : order{
public:
  newOrder(vector<string> tokens)
    : order(tokens),
      sym(tokens[2]),
      side(tokens[3])
  {}
  string sym;
  string side;
};

struct modifyOrder : order{
  modifyOrder(vector<string> tokens)
    : order(tokens),
      parent_orderid(stoi(tokens[2]))
  {}

  int parent_orderid;
};


vector<string> average_prices (const vector<string>& messages) {
  vector<string> result;

  for(const auto& msg : messages){
    trim(msg);
    unordered_map<int, order> orders;
    unordered_map<int, fill> fills;

    vector<string> tokens;
    split( tokens, msg, is_space(), token_compress_on);

    if (tokens[0] == "new"){
      newOrder ord(tokens);
      orders[ord.orderid] = ord;
    }else if (tokens[0] == "modify"){
      modifyOrder ord(tokens);
      orders[ord.orderid] = ord;
    }else if (tokens[0] == "fill"){
      fill fl(tokens);
      auto ord = orders[fl.orderid];
      while(ord is not newOrder){
        ord = orders[ord.orderid]
      }
      fills[ord.orderid]=fl;

      

    }
  return result;
}

int main()
{
  auto newOrd = parse<newOrder>("new 1 10");
    return 0;
}

