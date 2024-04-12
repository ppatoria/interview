# Types of OrderBook by order types.

| Order Type            | Regular Order                               | Trading Order                                    | Crossing Order                                               | Routing Order                                                  |
|-----------------------|---------------------------------------------|--------------------------------------------------|--------------------------------------------------------------|----------------------------------------------------------------|
| **Source**            | Individual traders or investors             | Institutional traders, algorithmic systems,      | Internal within a single brokerage firm or exchange,         | Traders seeking the best execution across multiple venues,     |
|                       | through a brokerage platform                | individual traders with advanced strategies      | often from institutional clients                             | sent to external exchanges or dark pools                       |
|                       |                                             |                                                  |                                                              |                                                                |
| **Purpose**           | Execute a trade at the current market price | Execute trades based on specific strategies,     | Match orders internally without impacting the broader market | Find the best price or                                         |
|                       |                                             | conditions, or price targets                     |                                                              | liquidity across different trading venues                      |
|                       |                                             |                                                  |                                                              |                                                                |
|                       |                                             |                                                  |                                                              |                                                                |
| **Differences**       | Most basic and common,                      | Variety of types                                 | Internal and not visible to the public market                | Seeking execution outside                                      |
|                       | without special conditions                  | with specific instructions for execution         |                                                              | the trader's primary platform                                  |
|                       |                                             |                                                  |                                                              |                                                                |
|                       |                                             |                                                  |                                                              |                                                                |
| **Destination**       | Primary exchange or                         | Can be primary exchange, ATS, or dark pools,     | Remains within the originating firm or exchange              | Sent to various exchanges, ECNs, or dark pools                 |
|                       | platform where the asset is listed          | depending on the order type                      |                                                              |                                                                |
|                       |                                             |                                                  |                                                              |                                                                |
| **Execution**         | Executed immediately                        | Executed based on the specific conditions set    | Executed at a mutually agreed price                          | Executed where the best price or liquidity is available        |
|                       | at the best available market price          |                                                  | between the matching parties                                 |                                                                |
|                       |                                             |                                                  |                                                              |                                                                |
| **Special Treatment** | No special treatment;                       | May receive priority                             | May have reduced fees or costs                               | May use smart order routing technologies to optimize execution |
|                       | follows standard market rules               | based on exchange rules for specific order types | due to internal execution                                    |                                                                |
|                       |                                             |                                                  |                                                              |                                                                |
| **Order Types Used**  | Market                                      | Market, Limit, Stop, Stop Limit, IOC, etc.       | Not applicable                                               | Market, Limit, Stop, Stop Limit, IOC, etc.                     |
|                       |                                             |                                                  | as it's a method of matching orders internally               | (depending on the routing strategy)                            |




# Design of OrderBook.
The provided code seems to be a part of a larger system, specifically related to an Order Book Strategy for a trading system. Here are the main data structures and their relationships:

1. **USOrderBookStrategy**: This is the main class that implements the `OrderBookStrategy` and `SecurityStatusListener` interfaces. It contains pointers to different types of order books (`USRegularOrderBook`, `USTXOrderBook`, `USCXOrderBook`, `USRXOrderBook`), a `USOrderBookStrategyFactory`, a `USOrderProcessor`, and a `USMdlFilter`. It also contains boolean flags `rule201_in_effect` and `luldAlwaysRequired`, and a `SecuritiesStatusInterface` pointer `CBInterface`.

2. **OrderBook**: The `OrderBook` is an abstract base class that represents a collection of buy and sell orders. It has derived classes like `USRegularOrderBook`, `USTXOrderBook`, `USCXOrderBook`, `USRXOrderBook` which override the `sort` method and the `is_allowed_to_cross_at_price` method. Each of these classes contains a pointer to `USOrderBookStrategy`.

3. **USOrderBookStrategyFactory**: This class is a factory for creating `USOrderBookStrategy` objects. It contains a vector `actReporterAvailability`, `utils::Time` objects `luldBeginTime` and `luldEndTime`, and a `MessageRoute` pointer `dcaRoute`.

4. **USOrderProcessor**: This class extends `OrderProcessor` and contains a pointer to `USOrderBookStrategy`.

5. **USMdlFilter**: This class extends `IOBMdlFilter` and contains a pointer to `USOrderBookStrategy`.

6. **BookSorter** and **BookSorterT**: These are functor classes used for sorting the order books. They compare `OrderData` objects based on various fields.

7. **TradeReportIDToTradeMatchIDTable**: This class maintains a mapping between `tradeReportID` and `tradeMatchID` in a hash map. It provides methods to add, erase, and find entries in the table.

8. **ACT90ReportParams**: This struct holds parameters for ACT90 reports. It contains various fields related to trades such as `tradeSide`, `qty`, `price`, `tradeTime`, etc.

9. **IOBACT90ComplianceReporter**: This class is responsible for creating compliance reports. It contains a pointer to `IACT90ReportSender` and a reference to `TradeReportIDToTradeMatchIDTable`.

Please note that without the complete code or documentation, it's hard to provide a comprehensive overview of the design and data structures. The actual implementation details of these classes and their methods are not provided in the code you've shared. Also, the headers for the base classes and other dependencies are not included, which might contain additional important information.
