# Question: Average Prices
* **SCENARIO**
The four space-separated message formats below describe the placement, modifying, and trading (i.e. "fill"ing) of orders at a securities exchange throughout the day:
new                                    <order-id> <security> <buy sell> <quantity> <price>
modify <order-id-from-new>             <order-id>                       <quantity> <price>
modify <order-id-from-earlier- modify> <order-id>                       <quantity> <price>
fill                                   <order-id>                       <quantity> <price>


* **Terminology:**
	* A chain (of orders) refers to a "new" order message, followed by 0 or more "modify"s, each referencing the previous message in the chain by order id. (Example below.)
	* The volume-weighted average price ("vwap") of one or more fill messages is the total money exchanged divided by the total number of shares exchanged.
	For example, fills for 5 shares at 100.00, 10 shares at 130.00, and 5 shares at 150.00, would have vwap of (5*100.00 + 10*130.00 + 5*150.00) / (5 +10+5)=(500.00 + 1300.00 + 750.00) / 20 = 2550.00/20 = 127.50.
	Due to race conditions at the exchange, a "fill" message's order-id can be from any "new" or "modify" message previously seen in the input messages.
* **TASK**
	* Provide efficient (in terms of big-O efficiency of operations on containers) and clean implementation for the function...
	```c++
	vector<string> average_prices (const vector<string>& messages);
	```
	... which is passed a sequence of exchange messages - as described above - and must return a vector containing strings with the format...
	<order-id> <total-fill-quanity> <vwap>
	...where:
	* <order-id>s are those seen in "new" messages, and appear in the returned vector<string> in the same order as in the "new" messages in the input
	* <total-fill-quantity> is the sum of the <quantity>s in "fill" messages for that chain of orders (or 0 if there are no fills)
	* <vwap> is the volume weighted average price (see example below) for the "fill"s on that chain of orders rounded down (or 0.00 if there are no fills).
* **EXAMPLE**
If you glance over the following messages (you don't need to consider all the naunces)...
    * new    1    IBM  buy  20 1250.00    order-id 1:	someone wants to buy 20 shares of IBM for <= $1250.00

    * modify 1    2    15   1240.00	      they decided 15 shares is enough, and only want to pay <= $1240.00 new 3 IBM buy 40 1245.00
	* fill   2    5    1240.00	          the original trader bought 5 shares for $1240.00 each

    * modify 2    4    15   1250.00
	* modify 4    5    15   1240.00	      The original trader wants to put the price back down now there's no competition to buy
	* fill   4    5    1250.00	          but before that took effect they'd already bought 5 at the higher price of $1250.00

    * modify 5    6    20   1240.00

    * new    3    IBM  buy  40 1245       order-id 3: distinct order to buy 40 shares of IBM for <= $1245.00
	* fill   3    40   1243.00	          order 3 above got all 40 shares for $1243.00 each

	* new    101  MSFT sell 40 100.00

	...there is a chain that includes order- ids 1, 2, 4, 5 and 6.
	order-id 3 is in a distinct chain by itself, as is order-id 101.
	The three fills are all for orders in the chain started by the "new" message with order-id 1, so that chain's vwap should be (5* 1240.00 + 40 * 1243.00 +5*1250.00) / (5 +40 +5)= 1243.40.
	The other chains don't have any fills.
	The returned vector should contain the three strings (without double-quotes), sorted as follows:
	"1 50 1243.40"
	"3 0 0.00" "101 0 0.00"
* **NOTES**
	* There will be 1 to 1,000,000 lines of input, for 1 to 1,000,000 distinct order ids, each of which is an integer between 0 and 1 billion (i.e. 1E9) inclusive.
	* All messages, fields and values will be valid / no validation is required or wanted.
	* Prices in dollars and cents, e.g. 1239.29.
	* No special effort need be made to avoid normal floating point errors, but do use double rather than floats
	* <quantity>s and <total-fill-quantity>s are integers between 1 and 1,000,000 (1E6) inclusive.
	* <price>s will be between 0.01 and 9999.99 inclusive.
	* Security names consist of 1 or more of the letters 'A' through 'Z'.

(1240 * 5) + (5 * 1250) + (40 * 1243) = 6200 + 6250 + 49720 = 62170 / 50 = 1243.40
6200, 5 


```c++
#include <bits/stdc++.h>
* Complete the 'average_prices' function below.
*
* The function is expected to return a STRING_ARRAY.
*The function accepts STRING_ARRAY messages as parameter.
*/
vector<string> average_prices (const vector<string>& messages) {
}
int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));
    vector<string> messages;
    string line;
    while (std::getline(std::cin, line))
    if (!line.empty()) messages.push_back(line);
    vector<string> result = average_prices (messages);
    for (const auto& chain_vwap: result)
        fout << chain_vwap << '\n';
}
```


