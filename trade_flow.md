# Trade flow:
The intricate dance of algorithmic trading in the context of equities.
Break down of the flow from the client's initial order to the final execution on the exchange, including the relevant components and interfaces.

1. **Client Order Initiation**:
   - The process begins when a client (such as an institutional investor or retail trader) decides to place an order to buy or sell equities.
   - The client sends the order to the investment bank via various channels (e.g., electronic trading platforms, phone, or FIX protocol).

2. **FIX Protocol**:
   - The **Financial Information eXchange (FIX)** protocol plays a crucial role in connecting the client's order management system (OMS) with the investment bank's trading infrastructure.
   - FIX messages facilitate communication between systems, including order placement, execution reports, and trade confirmations¹.

3. **Investment Bank's Trader Desk**:
   - The order reaches the trader desk, where human traders may review it, make adjustments, or decide to route it directly to the algorithmic trading framework.
   - The trader desk can also interact with the client for any clarifications or modifications.

4. **Algorithmic Trading Framework**:
   - The algorithmic trading framework houses various trading algorithms designed to execute orders efficiently.
   - Algorithms analyze market data, historical patterns, and real-time signals to make informed trading decisions.
   - The framework interfaces with the OMS and receives orders from the trader desk.

5. **Smart Order Routing (SOR) and Dark Pools**:
   - SOR algorithms determine the optimal execution venue (exchange, dark pool, or other liquidity sources) based on factors like liquidity, price, and market impact.
   - Dark pools are private venues where large orders can be executed anonymously, minimizing market impact.
   - The algorithmic framework routes orders to SOR or dark pools as needed.

6. **Exchange Connectivity Interface**:
   - The algorithmic framework connects to various exchanges via dedicated APIs (Application Programming Interfaces).
   - It sends orders to the exchange and receives execution reports.
   - The exchange connectivity layer handles order validation, matching, and execution.

7. **Execution on the Exchange**:
   - The exchange processes the order, matching it with other orders in the order book.
   - The trade is executed at the prevailing market price.
   - The exchange sends an execution report back to the algorithmic framework.

8. **Back to the Trading System**:
   - The execution report flows back through the exchange connectivity interface to the algorithmic trading framework.
   - The framework updates its internal state, accounting for the executed order.
   - The OMS receives the execution report, which may trigger further actions (e.g., notifying the client).

9. **Chain of Application**:
   - Throughout this flow, various applications (including risk management, compliance checks, and reporting) interact with the order.
   - The OMS maintains a record of the entire lifecycle of the order.

10. **Inclusion of OMS**:
    - The OMS (Order Management System) is a critical component that manages the entire order lifecycle.
    - It handles order creation, modification, cancellation, and execution tracking.
    - The OMS interacts with the trader desk, algorithmic framework, and exchange connectivity.

In summary, the journey from client order to execution involves FIX communication, trader decisions, algorithmic analysis, smart routing, exchange execution, and meticulous record-keeping. Algorithmic trading ensures efficiency, liquidity, and reduced human error, all while dancing to the rhythm of market dynamics.

# How the **FIX protocol** facilitates the connection between the client's **Order Management System (OMS)** and the investment bank's trading infrastructure.

1. **Role of the OMS**:
   - The **Order Management System (OMS)** is a critical component for buy-side firms (such as asset managers, hedge funds, or institutional investors).
   - The OMS handles the entire lifecycle of an order, from creation to execution and post-trade processing.
   - Functions of the OMS includes:
     - order entry
     - order routing
     - compliance checks
     - position management
     - reporting

2. **Connecting the Client's OMS to the Investment Bank**:
   - When a client (e.g., an asset manager) wants to execute a trade, they use their OMS to create an order.
   - The OMS generates a FIX message that encapsulates the order details (e.g., security, quantity, price, side) and other relevant information.
   - The FIX message confirms to the FIX protocol specifications.

4. **FIX Message Flow**:
   - The FIX message leaves the client's OMS and travels over a secure network (usually the internet) to reach the investment bank's trading infrastructure.
   - The FIX message contains essential fields, including:
     - **SenderCompID**  : The client's unique identifier.
     - **TargetCompID**  : The investment bank's unique identifier.
     - **Order details** : Security symbol, quantity, price, order type, etc.
     - **Message type**  : Indicates whether it's a new order, modification, or cancellation.
   - The FIX message is transmitted using TCP/IP or other reliable transport protocols.

5. **Investment Bank's Trading Infrastructure**:
   - The investment bank's trading infrastructure includes various components:
     - **FIX Engine**                : A software module that parses incoming FIX messages and routes them to the appropriate destination.
     - **Order Routing System**      : Determines where to route the order (e.g., specific algorithmic trading desk, dark pool, exchange).
     - **Risk Management**           : Performs pre-trade risk checks (e.g., position limits, credit checks).
     - **Matching Engine**           : Handles order matching and execution.
     - **Market Data Feed Handlers** : Receive real-time market data (prices, order book updates) from exchanges.
     - **Execution Reports**         : The investment bank generates FIX execution reports (e.g., "Order executed," "Order rejected") and sends them back to the client's OMS.

6. **FIX Message Parsing and Routing**:
   - The FIX engine at the investment bank receives the client's FIX message.
   - It parses the message, extracts relevant fields, and performs necessary validations.
   - The order routing system determines the appropriate destination (e.g., algorithmic trading desk).

7. **Algorithmic Trading Desk Interaction**:
   - If the order is routed to an algorithmic trading desk, the FIX message is passed to the relevant algorithm.
   - The algorithmic trading framework (written in C++, Python, or other languages) processes the order based on market conditions.
   - The algorithm may slice the order into smaller child orders for execution.

8. **Execution and Reporting**:
   - The algorithmic framework interacts with the exchange connectivity interface (via APIs) to execute the order.
   - Execution reports (including fill details) are generated and sent back to the client's OMS via FIX messages.
   - The OMS updates its records and notifies the client.

9. **End-to-End Transparency**:
   - FIX ensures end-to-end transparency by providing a standardized format for communication.
   - Both the client and the investment bank can track the order's progress using FIX messages.

In summary, the FIX protocol acts as the bridge between the client's OMS and the investment bank's trading infrastructure, enabling seamless communication and efficient order execution.

If you have any further questions or need additional clarification, feel free to ask!

# The role of **Order Management Systems (OMS)** in both buy-side and sell-side firms and understand why their importance differs:

1. **Buy-Side Firms (Asset Managers, Hedge Funds, etc.):**
   - **OMS Importance**: For buy-side firms, the OMS is indeed a critical component. Here's why:
     - **Order Lifecycle Management** : The OMS handles the entire lifecycle of an order, from creation to execution and post-trade processing.
     - **Trade Execution**            : It ensures efficient and accurate execution of trades across various asset classes (equities, fixed income, derivatives, etc.).
     - **Risk Management**            : The OMS performs pre-trade risk checks, monitors compliance with regulations, and manages exposure.
     - **Portfolio Management**       : It integrates with portfolio management systems, allowing portfolio managers to track positions and rebalance portfolios.
     - **Reporting and Audit Trails** : The OMS maintains detailed records of all orders and executions.
     - **Client Servicing**           : It facilitates communication with clients regarding order status and execution details.

2. **Sell-Side Firms (Broker-Dealers, Investment Banks, etc.):**
   - **OMS Importance**: While the OMS is essential for sell-side firms, its criticality may differ due to several factors:
     - **Trading Volume and Complexity**         : Sell-side firms handle a high volume of orders from multiple clients. Their OMS must efficiently manage this flow.
     - **Market Making and Proprietary Trading** : Sell-side firms engage in market making and proprietary trading. Their OMS supports these activities.
     - **Algorithmic Trading**                   : Sell-side firms use algorithms for execution. The OMS interfaces with algorithmic trading systems.
     - **Risk and Compliance**                   : Risk management and compliance checks are crucial, especially for market makers.
     - **Integration with Trading Desks**        : The OMS interacts with trading desks (e.g., equity, fixed income, foreign exchange) for execution.
     - **Multi-Asset Trading**                   : Sell-side firms trade across various asset classes. The OMS must handle equities, fixed income, derivatives, and more.
     - **Client Interaction**                    : The OMS provides real-time execution reports to clients.
     - **Efficiency and Scalability**            : Sell-side OMSes need to handle large volumes and scale seamlessly.

3. **Why OMS Matters for Sell-Side Firms**:
   - **Modernization and Flexibility** : Legacy OMS systems may lack modern features, flexibility, and scalability. Sell-side firms face growing volumes, shrinking margins, and technological innovation. Modern OMSes address these challenges¹.
   - **Multi-Asset Trading**           : Sell-side firms trade not only equities but also fixed income, derivatives, foreign exchange, and more. An OMS that supports multi-asset trading is crucial.
   - **Workflow Support**              : Sell-side OMSes must handle a full spectrum of workflows (high-touch, low-touch, risk) to maximize employee productivity and achieve vendor consolidation¹.
   - **Global Expansion**              : As sell-side firms expand globally, their OMS must support regional workflows, localized expertise, and on-the-ground support¹.

In summary, while the OMS is vital for both buy-side and sell-side firms, the specific requirements and challenges faced by sell-side entities make OMS modernization and flexibility critical for their success in today's dynamic markets.

# Break down of the process of determining the appropriate destination for an order within the context of an investment bank's trading infrastructure.

We'll explore how the system decides whether to route an order to a **trader desk** or an **algorithmic trading desk**.
I'll illustrate this with real-world scenarios and step-by-step details:

### **Order Routing Decision Process: Trader Desk vs. Algorithmic Trading Desk**

1. **Order Creation**:
   - A client (e.g., an asset manager) initiates an order through their **Order Management System (OMS)**.
   - The OMS generates a FIX message containing order details (security, quantity, price, etc.).

2. **Order Routing System**:
   - The order routing system receives the FIX message from the OMS.
   - Its role is to determine where to route the order for execution.

3. **Static vs. Dynamic Routing**:
   The system evaluates various factors to make the routing decision:
   - **Order Characteristics** : Is it a market order, limit order, or a specific algorithmic order?
   - **Client Preferences**    : Does the client have specific routing preferences?
   - **Market Conditions**     : What is the current market volatility, liquidity, and trading volume?
   - **Security Type**         : Different securities (equities, options, futures) may require different handling.
   - **Static Routing**        :
   - Some orders are pre-configured to follow specific routes (e.g., always go to the trader desk).
   - Static routing is based on fixed rules set by the firm.
   - **Dynamic Routing**       :
   - Dynamic routing adapts to real-time conditions.
   - It uses algorithms to make decisions based on market data and order characteristics.

4. **Trader Desk**:
   - **Scenario 1: High-Touch Order**:
     - If the order requires personalized attention (e.g., large block orders, special instructions), it goes to the trader desk.
     - Reasons for routing to the trader desk:
       - **Complex Orders**     : Orders with specific conditions (e.g., "execute only during market open").
       - **Client Interaction** : The client wants to discuss execution strategy.
       - **Customization**      : The trader can tailor execution based on market conditions.
   - **Scenario 2: Market Making**:
     - For market-making activities, the trader desk handles liquidity provision.
     - The trader quotes bid and ask prices, managing inventory and risk.

5. **Algorithmic Trading Desk**:
   - **Scenario 1: Algorithmic Orders**:
     - If the order is suitable for algorithmic execution, it goes to the algorithmic trading desk.
     - Reasons for routing to the algorithmic desk:
       - **High Volume**       : Smaller slices of a large order.
       - **Minimizing Impact** : Algorithms aim to minimize market impact.
       - **VWAP, TWAP, etc.**  : Specific algorithmic strategies.
   - **Scenario 2: Real-Time Adaptation**:
     - Algorithms adapt to changing market conditions (volatility, liquidity).
     - They dynamically adjust execution parameters (slice size, timing).

6. **Algorithm Selection**:
   - The system selects an appropriate algorithm based on the order type and market conditions.
   - Common algorithms:
     - **VWAP (Volume-Weighted Average Price)** : Execute over a specified time while minimizing impact.
     - **TWAP (Time-Weighted Average Price)**   : Execute evenly over a fixed time period.
     - **Implementation Shortfall**             : Minimize deviation from a benchmark price.

7. **Execution and Reporting**:
   - The algorithmic trading desk interfaces with the exchange connectivity.
   - Execution reports flow back to the OMS.
   - The OMS updates the client on order status.

8. **Dynamic Data Usage**:
   - **Real-Time Market Data**: Algorithms use real-time prices, volume, and liquidity.
   - **Adaptive Logic**: Algorithms adjust based on intraday volatility and liquidity.

### **Trade Flow Summary**:
1. Client initiates an order.
2. Order routing system evaluates order characteristics.
3. If personalized attention needed, route to trader desk; else, to algorithmic desk.
4. Algorithmic desk selects an appropriate algorithm.
5. Algorithm executes order, adapting to real-time conditions.
6. Execution reports flow back to the OMS.

Remember that the choice between trader desk and algorithmic desk depends on the specific order, client preferences, and market dynamics. The system aims to balance efficiency, liquidity, and client needs.

# Market Making (MM) and Proprietary Trading and how sell-side firms engage in these activities:
1. **Market Making (MM)**:
   - **Definition**: Market makers are financial institutions (often sell-side firms) that provide liquidity to financial markets by continuously quoting bid and ask prices for specific securities.
   - **Role of Market Makers**:
     - **Liquidity Provision** : Market makers ensure that there are always buyers and sellers for a given security.
     - **Bid-Ask Spread**      : They profit from the bid-ask spread (the difference between the buying and selling price).
     - **Risk Management**     : Market makers manage inventory and risk exposure.
   - **How It Works**:
     - A market maker continuously quotes both buy (bid) and sell (ask) prices for a specific security.
     - When a trader wants to buy or sell, they can execute the trade with the market maker.
     - Market makers maintain an inventory of the security and adjust their quotes based on supply and demand.
     - They aim to minimize the spread while managing risk.
   - **Examples**: Market makers operate in stock exchanges, options markets, and foreign exchange markets.

2. **Proprietary Trading**:
   - **Definition**: Proprietary trading (often called "prop trading") refers to trading financial instruments using a firm's own capital rather than client funds.
   - **Role of Prop Traders**:
     - Prop traders seek to profit from short-term price movements, arbitrage opportunities, or market inefficiencies.
     - They use various strategies, including statistical arbitrage, high-frequency trading, and event-driven trading.
     - Prop trading desks are typically part of sell-side firms.
   - **How It Works**:
     - Prop traders analyze market data, identify patterns, and execute trades.
     - They may hold positions for seconds (high-frequency trading) to days (swing trading).
     - Prop trading desks have access to advanced technology, real-time data feeds, and sophisticated algorithms.
     - Profit and loss directly impact the firm's bottom line.
   - **Risk and Regulation**:
     - Prop trading involves risk, as gains and losses are borne by the firm.
     - Regulatory restrictions (such as the Volcker Rule) limit proprietary trading by banks to prevent excessive risk-taking.
   - **Examples**: Proprietary trading desks engage in equities, fixed income, derivatives, and other asset classes.

3. **Sell-Side Firms and MM/Prop Trading**:
   - **Market Making**:
     - Sell-side firms act as market makers by providing liquidity to clients and exchanges.
     - They quote bid and ask prices, facilitating smooth trading.
     - Market-making desks handle inventory, risk, and execution.
   - **Proprietary Trading**:
     - Sell-side firms engage in prop trading to generate profits.
     - Prop traders use the firm's capital to execute strategies.
     - These desks operate within risk limits and compliance guidelines.

4. **Balancing Act**:
   - Sell-side firms balance their roles as market makers, proprietary traders, and intermediaries.
   - They manage risk, comply with regulations, and optimize profitability.

In summary, market making ensures liquidity, while proprietary trading allows sell-side firms to profit from their expertise and capital. Both activities contribute to the vibrancy of financial markets.

If you'd like further examples or have additional questions, feel free to ask!

# The different types of orders in trading and explore what makes algorithmic orders unique compared to market, limit, and stop orders:

1. **Market Order**:
   - A **market order** is an instruction to buy or sell a security immediately at the best available market price.
   - Key characteristics:
     - **Execution**            : Market orders prioritize speed over price.
     - **Price**                : The order executes at the prevailing market price (whatever buyers or sellers are willing to pay).
     - **Guaranteed Execution** : Market orders are guaranteed to execute but not guaranteed at a specific price.
   - Example: "Buy 100 shares of XYZ stock at the current market price."

2. **Limit Order**:
   - A **limit order** specifies a price at which you are willing to buy or sell a security.
   - Key characteristics:
     - **Price Control**          : You set a specific price (limit) for execution.
     - **Execution Condition**    : The order will only execute if the market reaches your specified price or better.
     - **No Execution Guarantee** : While limit orders provide price control, execution is not guaranteed.
   - Example: "Sell 50 shares of ABC stock at a limit price of $75 per share."

3. **Stop Order (Stop-Loss or Stop-Buy)**:
   - A **stop order** triggers a market order when a specific price level (the stop price) is reached.
   - Key characteristics:
     - **Activation**        : The order becomes a market order when the stop price is hit.
     - **Risk Management**   : Stop orders are commonly used for risk management (e.g., stop-loss orders).
     - **Price Uncertainty** : Like market orders, execution price is uncertain.
   - Example:
     - "Sell 200 shares of DEF stock if the price drops to $60 per share."
     - "Buy 100 shares of GHI stock if the price rises to $90 per share."

4. **Specific Algorithmic Orders**:
   - **Algorithmic orders** refer to orders executed using predefined trading algorithms.
   - These orders are designed to achieve specific objectives beyond simple execution.
   - Examples of algorithmic orders:
     - **VWAP (Volume-Weighted Average Price)** : Execute over a specified time while minimizing market impact.
     - **TWAP (Time-Weighted Average Price)**   : Execute evenly over a fixed time period.
     - **Implementation Shortfall**             : Minimize deviation from a benchmark price.
     - **Iceberg Orders**                       : Conceal large orders by executing in smaller visible portions.
     - **Pegged Orders**                        : Track the best bid or ask price dynamically.
     - **Dark Pool Orders**                     : Execute anonymously in dark pools.
   - **Differences from Basic Orders**:
     - **Objective**          : Algorithmic orders have specific execution goals (e.g., minimizing impact, achieving a benchmark).
     - **Dynamic Adaptation** : Algorithms adjust based on real-time market conditions.
     - **Complex Logic**      : Algorithmic orders involve sophisticated logic beyond simple buy/sell instructions.

In summary:
- **Market orders**      : prioritize speed and execute at the best available price.
- **Limit orders**       : allow price control but may not guarantee execution.
- **Stop orders**        : trigger market orders based on a specified price level.
- **Algorithmic orders** : use predefined strategies for specific objectives.

Remember that the choice of order type depends on the trader's goals, risk tolerance, and market conditions.

If you'd like further examples or have additional questions, feel free to ask!

# The concept of risk management for market makers and explore what it means to manage inventory and risk exposure:

1. **Market Makers and Risk Management**:
   - **Market makers** are financial institutions (often sell-side firms) that provide liquidity by continuously quoting bid and ask prices for specific securities.
   - Their role is crucial in ensuring smooth trading and efficient markets.
   - However, market making involves risks, and effective risk management is essential.

2. **Inventory and Risk Exposure**:
   - **Inventory**: Market makers hold an inventory of securities (stocks, bonds, derivatives) that they are willing to buy or sell.
   - **Risk Exposure**: This refers to the potential losses a market maker faces due to adverse price movements or unexpected events.
   - Let's break down how market makers manage these aspects:

3. **Managing Inventory**:
   - **Accumulating Inventory**:
     - Market makers continuously quote bid and ask prices.
     - When a trader sells (market order), the market maker accumulates inventory.
     - When a trader buys (market order), the market maker reduces inventory.
   - **Balancing Inventory**:
     - Market makers aim to maintain balanced inventory.
     - Too much inventory exposes them to risk (e.g., if prices drop).
     - Too little inventory affects their ability to provide liquidity.
   - **Hedging Strategies**:
     - Market makers use hedging techniques to offset risk.
     - For example, if they accumulate long positions, they may hedge by shorting related securities.
     - Hedging reduces directional risk.

4. **Risk Exposure Management**:
   - **Price Risk**:
     - Market makers face price risk due to market fluctuations.
     - They monitor market data (prices, order book) to assess exposure.
     - Algorithms adjust quotes based on real-time data.
   - **Volatility Risk**:
     - High volatility increases risk exposure.
     - Market makers adjust spreads and inventory size accordingly.
   - **Event Risk**:
     - Unexpected events (earnings announcements, geopolitical news) impact prices.
     - Risk management includes adjusting inventory before such events.
   - **Liquidity Risk**:
     - Market makers must be prepared to execute large orders without significant price impact.
     - Managing liquidity risk involves sizing inventory appropriately.

5. **Dynamic Adjustments**:
   - Market makers continuously adapt:
     - **Quote Width**: Adjusting bid-ask spreads based on volatility.
     - **Position Limits**: Setting maximum inventory levels.
     - **Risk Limits**: Monitoring exposure against predefined limits.
     - **Algorithmic Trading**: Using algorithms to optimize execution.

6. **Profit and Loss (P&L)**:
   - Market makers aim for consistent P&L while managing risk.
   - Profit comes from bid-ask spreads and trading volume.
   - Losses occur if inventory moves against them.

7. **Overall Objective**:
   - Market makers seek to provide liquidity while minimizing risk.
   - Effective risk management ensures their long-term viability.

In summary, market makers play a vital role in maintaining market liquidity, but they must carefully manage inventory and risk exposure to thrive in dynamic financial markets.

If you have further questions or need additional clarification, feel free to ask!

# Market making, while essential for maintaining liquidity, involves several risks that market makers must manage. Let's explore these risks in detail:

1. **Price Risk**:
   - **Description**           : Market makers face price risk due to fluctuations in security prices.
   - **Risk Exposure**         : Holding inventory exposes market makers to potential losses if prices move against their positions.
   - **Mitigation Strategies** :
     - Continuously monitor market data (prices, order book) to assess exposure.
     - Use algorithms to dynamically adjust quotes based on real-time data.
     - Hedge positions to offset directional risk.

2. **Volatility Risk**:
   - **Description**           : High volatility increases the uncertainty of price movements.
   - **Risk Exposure**         : Market makers must adjust spreads and inventory size to account for volatile market conditions.
   - **Mitigation Strategies** :
     - Widen spreads during high volatility to compensate for increased risk.
     - Adjust inventory levels to match market conditions.

3. **Event Risk**:
   - **Description**           : Unexpected events (earnings announcements, geopolitical news) impact security prices.
   - **Risk Exposure**         : Market makers must be prepared for sudden price movements.
   - **Mitigation Strategies** :
     - Adjust inventory before significant events.
     - Monitor news and economic calendars.

4. **Liquidity Risk**:
   - **Description**           : Market makers must be ready to execute large orders without significant price impact.
   - **Risk Exposure**         : Illiquid markets can lead to wider spreads and difficulty in unwinding positions.
   - **Mitigation Strategies** :
     - Maintain sufficient inventory to handle large trades.
     - Balance liquidity provision with risk tolerance.

5. **Counterparty Risk**:
   - **Description**           : Market makers face credit risk from counterparties.
   - **Risk Exposure**         : If a counterparty defaults, market makers may incur losses.
   - **Mitigation Strategies** :
     - Assess counterparty creditworthiness.
     - Diversify counterparties.

6. **Regulatory Risk**:
   - **Description**           : Regulatory changes can impact market making activities.
   - **Risk Exposure**         : New rules may affect market structure, capital requirements, or proprietary trading.
   - **Mitigation Strategies** :
     - Stay informed about regulatory developments.
     - Adapt strategies to comply with changing rules.

7. **Technological Risk**:
   - **Description**           : Technology failures can disrupt market making operations.
   - **Risk Exposure**         : System outages, connectivity issues, or algorithmic glitches.
   - **Mitigation Strategies** :
     - Robust technology infrastructure.
     - Backup systems and redundancy.

8. **Competition Risk**:
   - **Description**           : Market makers compete with each other.
   - **Risk Exposure**         : Tighter spreads reduce profit margins.
   - **Mitigation Strategies** :
     - Innovate to maintain a competitive edge.
     - Optimize efficiency.

9. **Profit and Loss (P&L) Volatility**:
   - **Description**           : Market makers' P&L can be volatile due to changing market conditions.
   - **Risk Exposure**         : Unexpected losses impact profitability.
   - **Mitigation Strategies** :
     - Diversify strategies.
     - Set risk limits.

10. **Overall Objective**:
    - Market makers aim to balance providing liquidity with managing risk.
    - Effective risk management ensures long-term sustainability.

In summary, market making involves a delicate balance between liquidity provision and risk mitigation. Market makers continuously adapt to changing market dynamics while safeguarding their positions.


# The details of **stop-loss orders**, how they work, and why traders use them for risk management:

1. **What Is a Stop-Loss Order?**
   - A **stop-loss order** is an instruction given by a trader to their broker.
   - It serves as a risk management tool to limit potential losses on an existing position.
   - When the security's price reaches a specified level (the **stop price**), the stop-loss order becomes a market order and executes at the next available opportunity.

2. **How Stop-Loss Orders Work**:
   - **Sell Stop-Loss Order**:
     - If you hold a long position (you own the security), you can set a sell stop-loss order.
     - It instructs your broker to sell the security if its market price drops to or below the specified stop price.
     - This helps limit potential losses.
   - **Buy Stop-Loss Order**:
     - If you hold a short position (you've borrowed and sold the security), you can set a buy stop-loss order.
     - It sets the stop price above the current market price.
     - If the security's price rises to or above the stop price, the order triggers a market order to buy, closing out your short position.

3. **Advantages of Stop-Loss Orders**:
   - **Risk Control**             : Stop-loss orders allow you to define a maximum acceptable loss.
   - **Automatic Execution**      : Once the stop price is met, the order becomes a market order and executes promptly.
   - **Long and Short Positions** : Stop-loss orders work for both long and short positions.

4. **Comparison with Stop-Limit Orders**:
   - **Stop-Loss Order**:
     - Becomes a market order when the stop price is reached.
     - Always executes (assuming there are buyers or sellers).
     - Removes the risk of not closing out the position as the price continues to fall.
   - **Stop-Limit Order**:
     - Also triggers at the stop price.
     - However, it is an order to execute at a specific (limit) price.
     - May not execute if the limit price is not reached.

5. **Potential Disadvantages**:
   - **Price Gaps** : Stop-loss orders may execute at unfavorable prices during extreme market volatility or gaps.
   - **Whipsaws**   : In volatile markets, prices can quickly move above and below the stop price, triggering unnecessary orders.

6. **Example**:
   - Suppose you buy 100 shares of XYZ stock at $50 per share.
   - You set a stop-loss order with a stop price of $45 (10% below your purchase price).
   - If the stock price drops to $45 or below, the stop-loss order triggers, and your shares are sold at the best available price.

In summary, stop-loss orders are essential risk management tools that help traders limit losses and protect profits. They are widely used in both long and short positions.

# The mitigation strategies for market makers (MMs) facing price risk and explore how they handle scenarios where the price of a security drops significantly, and market sentiment turns negative.Break down into simple terms:

### **Price Risk Mitigation Strategies for Market Makers:**

1. **Continuous Monitoring**:
   - **What It Means**: MMs continuously track market data, including real-time prices and the order book.
   - **Why It Matters**: By staying informed, MMs can assess their exposure to price movements.
   - **Scenario**:
     - Suppose an MM holds an inventory of XYZ stock.
     - Suddenly, negative news about the company causes the stock price to plummet.
   - **Mitigation**:
     - The MM quickly adjusts their quotes (bid and ask prices) to reflect the new market reality.
     - They may widen the spread to account for increased risk.

2. **Algorithmic Adjustments**:
   - **What It Means**  : MMs use algorithms to dynamically adjust their quotes based on real-time data.
   - **Why It Matters** : Algorithms respond faster than manual adjustments.
   - **Scenario**:
     - The stock price continues to drop due to panic selling.
   - **Mitigation**:
     - The algorithm automatically widens spreads to avoid aggressive execution.
     - It may even reduce inventory by selling some shares.

3. **Hedging Positions**:
   - **What It Means**  : MMs hedge their positions to offset directional risk.
   - **Why It Matters** : Hedging reduces exposure to price movements.
   - **Scenario**       :
     - XYZ stock faces heavy selling pressure, and there are no buyers.
   - **Mitigation**     :
     - The MM may short-sell related securities (e.g., sector ETFs) to offset losses.
     - This way, if the entire sector is down, the MM's overall position is less affected.

4. **Scenario: Illiquid Market**:
   - **Description**           : Imagine a scenario where XYZ stock becomes illiquid—few buyers and sellers.
   - **Risk Exposure**         : The MM's inventory is stuck, and they can't easily unwind positions.
   - **Mitigation Strategies** :
     - **Patience**          : The MM waits for liquidity to return.
     - **Gradual Unwinding** : If possible, they sell small portions over time.
     - **Diversification**   : MMs diversify their inventory to avoid concentration risk.

5. **Scenario: Extreme Volatility**:
   - **Description**           : The stock experiences extreme price swings.
   - **Risk Exposure**         : MMs face higher price risk.
   - **Mitigation Strategies** :
     - **Wider Spreads**                : MMs increase bid-ask spreads to compensate for volatility.
     - **Dynamic Inventory Management** : They adjust inventory size based on volatility.

6. **Scenario: No Buyers**:
   - **Description**           : XYZ stock has no buyers due to negative sentiment.
   - **Risk Exposure**         : The MM can't sell their inventory.
   - **Mitigation Strategies** :
     - **Risk Tolerance**  : MMs set risk limits and accept that some positions may be illiquid.
     - **Diversification** : Holding a mix of securities reduces reliance on any single stock.

In summary, MMs actively manage their inventory, adapt to changing market conditions, and use algorithms to mitigate price risk. However, extreme scenarios (like a complete lack of buyers) can still pose challenges. Risk management is a delicate balance between liquidity provision and exposure control.


# **Stop-loss orders** are not limited to day trading; they can be set for longer durations, including daily carry-forward.:

1. **Day Trading**:
   - In day trading, traders open and close positions within the same trading day.
   - Stop-loss orders are commonly used to manage risk during day trades.
   - These orders are valid only for the current trading session.

2. **Carry-Forward Stop-Loss Orders**:
   - **What They Are** : Carry-forward stop-loss orders remain active beyond the current trading day.
   - **How They Work** :
     - You set a stop-loss order with a specific stop price (e.g., 10% below your purchase price).
     - If the security's price reaches the stop price, the order triggers.
     - The stop-loss order remains active until executed or canceled.
   - **Scenario**      :
     - Suppose you buy 100 shares of ABC stock at $50 per share.
     - You set a stop-loss order with a stop price of $45 (10% below your purchase price).
     - If the stock price drops to $45 or below, the stop-loss order triggers, regardless of the trading day.

3. **Advantages of Carry-Forward Stop-Loss Orders**:
   - **Risk Management**     : They help limit losses beyond intraday trading.
   - **Long-Term Positions** : Carry-forward orders work for both short-term and long-term positions.
   - **Automatic Execution** : Once the stop price is met, the order executes promptly.

4. **Considerations**:
   - **Market Conditions** : Be aware of overnight gaps (price jumps) that can trigger stop-loss orders.
   - **Review Regularly**  : Adjust stop prices as needed based on changing market conditions.

In summary, stop-loss orders can be set once and carried forward daily, providing risk management beyond day trading.

# Trade flow Algorithmic orders.
The intricacies of algorithmic trading, considering the software development aspects, the flow from the algorithmic trading platform to the exchange, and the use of common algorithms like VWAP (Volume-Weighted Average Price).
I'll break it down step by step:

1. **Software Developer and Algo Developer Roles**:
   - A **software developer** typically works on building and maintaining the infrastructure, including the algorithmic trading platform, connectivity, and risk management systems.
   - An **algo developer** focuses on designing, implementing, and optimizing trading algorithms. They write the code that drives the decision-making process.

2. **Flow from Algorithmic Trading Platform to Exchange**:
   - Let's follow the journey of an order from the algorithmic trading platform to the exchange and back:
     1. **Algorithmic Trading Framework (C++)**:
        - The algorithmic framework, written in C++, hosts various trading algorithms.
        - It receives orders from the OMS (Order Management System) or directly from the trader desk.
        - The framework interfaces with market data feeds (real-time prices, order book updates, etc.).

     2. **Algorithm Implementation (Python)**:
        - The actual trading algorithms are written in Python (or other languages).
        - Common algorithms include VWAP, TWAP (Time-Weighted Average Price), and implementation shortfall.
        - The algo developer designs and codes these strategies.

     3. **Algorithm Execution**:
        - The algorithmic framework receives an order (e.g., "Buy 1000 shares of XYZ stock").
        - The selected algorithm (e.g., VWAP) processes the order based on market conditions.
        - The algorithm slices the order into smaller child orders for execution.

        Break down of the **Algorithm Execution** process for a VWAP (Volume Weighted Average Price) algorithm within an investment bank's trading platform.
        We'll consider the example of buying 1000 shares of XYZ stock:

        1. **Decision-Making Process**:
          - **Objective** : Buy 1000 shares while tracking the VWAP.
          - **Context**   : An institutional investor wants to execute the order over a day.
          - **Options**   :
            - **Market Order**   : Execute immediately at the current market price.
            - **VWAP Algorithm** : Gradually execute to match the average market price.

        2. **Static and Dynamic Data Analysis**:
          - **Static Data**:
            - **Order Details**   : Buy 1000 shares of XYZ stock.
            - **Historical Data** : Historical price and volume data for XYZ stock.
          - **Dynamic Data**:
            - **Real-Time Market Data** : Order book updates, current prices, and volume.
            - **VWAP Calculation**      : Continuously compute the VWAP based on executed trades.
              1. **VWAP Calculation**:
                - VWAP represents the average price at which a security has traded throughout the day, considering both volume and price.
                - The formula for VWAP is:
                  \[ \text{VWAP} = \frac{\text{Cumulative Typical Price} \times \text{Volume}}{\text{Cumulative Volume}} \]
                  - **Typical Price** : Calculated as the average of the high price, low price, and closing price for each trade.
                  - **Cumulative**    : Represents the total since the trading session opened.

              2. **Continuous Computation**:
                - VWAP is recalculated **continuously** throughout the trading day.
                - At the start of each new trading session (e.g., each day), the VWAP resets.
                - As new trades occur, the cumulative values are updated.

              3. **Implementation**:
                - **Streaming Chart Integration**:
                  - VWAP is added to a streaming chart.
                  - The calculation is made automatically as new trades arrive.
                - **Manual Calculation**:
                  - To calculate VWAP manually:
                    - Find the average price for the first time interval (e.g., the first five minutes of the day).
                    - Multiply this average price by the volume traded during that interval (resulting in the "PV" value).
                    - Divide PV by the volume for that interval to obtain the VWAP for that interval.
                    - To maintain VWAP throughout the day, continue adding PV values from each interval to the prior values.
                    - Divide the total PV by the total volume up to that point.

              4. **Use Cases**:
                - **Trend Confirmation Tool**:
                  - Traders use VWAP to confirm trends.
                  - Stocks below VWAP may be considered undervalued, while those above it may be overvalued.
                - **Institutional Buyers**:
                  - Mutual funds and institutions use VWAP to minimize market impact.
                  - They aim to buy below VWAP and sell above it.

              5. **Real-Time Example**:
                - Imagine a trader monitoring VWAP throughout the day:
                  - At each interval (e.g., every 5 minutes), calculate the VWAP based on executed trades.
                  - Adjust the VWAP as new trades occur.
                  - Use this real-time VWAP to inform trading decisions.
              Remember, VWAP provides valuable pricing insight and helps traders balance execution efficiency with market impact.

        3. **Slicing and Scheduling**:
          - **Slicing Strategy**:
            - Divide the 1000-share order into smaller slices (e.g., 100 shares each).
            - Slice size depends on market conditions, liquidity, and risk tolerance.
              - **Market Conditions :** Suppose the stock market is very volatile.
                                        This might encourage you to slice your order into smaller parts to minimize the impact on the stock price.
              - **Liquidity         :** You notice that the stock is highly liquid.
                                        This means you can execute your slices quickly without significantly impacting the stock’s price.
              - **Risk Tolerance    :** If you have a high risk tolerance, you might choose to execute larger slices despite the market volatility.
                                        Conversely, if you have a low risk tolerance, you might choose to execute smaller slices to reduce your risk.
              - **VWAP Strategy     :** You decide to use a VWAP strategy to execute your order.
                                        This means you aim to execute your order close to the VWAP of the stock.
                                        To do this, you might choose to execute larger slices when the stock price is below the VWAP and smaller slices when the stock price is above the VWAP.
          - **Scheduling Execution**:
            - **Schedule slices at regular intervals. Depending on the following :**
                Note VWAP analysis does take into account the volume of trades that occur during different time slots.
                The "V" in VWAP stands for "Volume", and it means that the average price is weighted according to the volume of trades that happen at each price level.
                In terms of scheduling execution:
                - **High Volume Time Slots**: If the historical data shows that certain time slots consistently have high trading volumes, the algorithm might choose to execute larger slices during these times.
                                              This is because high volume periods offer better liquidity, which can help minimize the impact of large trades on the market price.
                - **Low Volume Time Slots**:  Conversely, during times of low volume, the algorithm might choose to execute smaller slices to avoid influencing the market price too much.
            - **Use real-time VWAP calculations to adjust slice prices.**
              The algorithm can adjust the slice prices in real-time based on the VWAP calculations.
              If the current price is below the VWAP, it might choose to execute a larger slice, and vice versa.
            - **Place limit orders for each slice.**
              Placing limit orders for each slice helps to control the execution price.
              The algorithm can adjust the limit price based on real-time market conditions and the VWAP.

        4. **Execution Actions**:
          - For each slice:
            - **Order Placement**:
              - Place a limit order for 100 shares at a price near the current VWAP.
            - **Monitoring**: It tracks order execution progress.
              Monitor the order book for execution.
              When an order is executed, the broker typically sends an execution report to the trader, which provides details about the execution, including the price, quantity, time, and venue of execution.
              This is the primary way that traders learn of their order's execution.
              However, monitoring the order book can still be useful for several reasons:
              - **Anticipating order execution**: By watching the order book, a trader can see if their order is likely to be executed soon.
                  For example, if their limit order is at the top of the book, it will likely be executed once a matching order arrives.
              - **Understanding market conditions**:
                The order book can provide valuable information about the supply and demand for a security, which can help the trader make informed decisions.
              - **Algorithmic trading**: In algorithmic trading, where orders may be split into smaller "child" orders to minimize market impact, monitoring the order book can help the algorithm decide when and where to place these child orders.
              So while it's not strictly necessary to monitor the order book to know when an order has been executed, doing so can provide additional insights that can be valuable for trading.
            - **Executed and Filled**:
              - If the order executes successfully, update the remaining shares.
              - Track the executed price relative to the VWAP.
                VWAP stands for Volume Weighted Average Price, which gives the average price a security has traded at throughout the day, based on both volume and price.
                It’s important because it provides traders with insight into both the trend and value of a security.
                Tracking the executed price relative to the VWAP means comparing the price at which your order was filled to the VWAP.
                If the executed price is lower than the VWAP for a buy order (or higher for a sell order), it means you’ve achieved a better-than-average price.
            - **Canceled**:
              - If a slice is canceled (e.g., due to sudden price movement), adjust the remaining slices.
            - **Rejected**:
              - Rejections can occur due to insufficient liquidity or price constraints.
                While it’s true that market orders are more likely to be rejected due to insufficient liquidity or price constraints (because they’re looking for immediate execution), limit orders can also be rejected.
                For example, a limit order could be rejected if it’s placed at a price that’s significantly away from the current market price, as it’s unlikely to be filled.
                In such cases, the trading algorithm may need to adjust the price or size of the order and resubmit it.
              - Handle rejections by modifying the slice (e.g., adjusting price or size) and re-submitting.

        5. **Connection Failure**:
          - If the connection between the IB and the exchange goes down:
            - Executed slices remain unaffected.
            - Remaining slices wait until the connection is restored.

        6. **Market Condition Deviation**:
          - If market conditions deviate from analysis:
            - **Adaptation**:
              - Adjust slice sizes or timing based on real-time data.
              - Reassess the VWAP target.
            - **Risk Management**:
              - Consider stopping or modifying the remaining slices.
              - Monitor market impact and adjust strategy.

        7. **Scenario Example**:
          - **Initial Order**: Buy 1000 shares of XYZ stock.
          - **Slicing**: Divide into 10 slices of 100 shares each.
          - **Execution**:
            - Slice 1: Place a limit order for 100 shares at a price near the current VWAP.
            - Slice 2: Repeat for the next interval.
            - ...
            - Slice 10: Execute the final slice.
          - **Monitoring**:
            - Track executed prices, VWAP deviation, and market impact.
            - Adjust remaining slices as needed.

        Remember, the VWAP algorithm aims to minimize market impact while achieving the desired average price.
        It adapts to real-time conditions and balances execution efficiency with risk management.



     4. **Smart Order Routing (SOR)**:
        - If the algorithm decides to route the order externally (e.g., to an exchange), SOR comes into play.
        - SOR algorithms evaluate liquidity, fees, and market impact to choose the best execution venue.
        - The order is routed to the appropriate exchange or dark pool.

     5. **Exchange Connectivity Interface**:
        - The algorithmic framework communicates with the exchange via APIs.
        - It sends the child orders to the exchange's matching engine.
        - The exchange validates the order and executes it against existing orders.

     6. **Execution on the Exchange**:
        - The exchange matches the order with other market participants.
        - The trade occurs at the prevailing market price.
        - The exchange sends an execution report back to the algorithmic framework.

     7. **Back to the Algorithmic Framework**:
        - The execution report flows back through the exchange connectivity interface.
        - The framework updates its internal state (e.g., position, P&L).
        - Risk checks (e.g., position limits, credit limits) are performed.

     8. **OMS Interaction**:
        - The OMS receives the execution report.
        - It updates the order status (filled, partially filled, rejected).
        - The OMS may notify the trader desk or the client.

3. **VWAP Algorithm Example**:
   - The VWAP algorithm aims to execute an order over a specified time horizon while minimizing market impact.
   - It calculates the volume-weighted average price based on historical data.
   - The algo developer codes the VWAP logic in Python.
   - The algorithm slices the order into smaller chunks and executes them at intervals to achieve the desired VWAP.

4. **Language Choice**:
   - The algorithmic framework in C++ provides low-latency performance.
   - Python is chosen for algorithm development due to its expressiveness, libraries (NumPy, pandas), and ease of prototyping.

# order book updates and their significance in trading:

1. **Order Book Updates**:
   - The **order book** is a dynamic record of buy and sell orders in a financial market. It displays the outstanding limit orders at various price levels.
   - **Updates** occur when new orders are added, existing orders are modified, or orders are removed from the book.
   - Common types of updates include:
     - **Insertions**    : New limit orders are added to the book.
     - **Modifications** : Existing orders change their size or price.
     - **Deletions**     : Orders are removed from the book (e.g., filled or canceled).

2. **Use of Order Book Updates**:
   - **Decision Making**:
     - **Liquidity Assessment**    : Traders analyze order book depth to gauge liquidity. A thick order book suggests ample liquidity, while a thin one indicates potential slippage.
     - **Price Impact Prediction** : Order book updates help estimate the impact of large market orders. A sudden change in the book can affect prices.
     - **Market Sentiment**        : Patterns in the order book reveal sentiment. For instance, a cluster of buy orders at a specific level may signal bullishness.
   - **Slicing and Execution**:
     - **Slicing Strategies**       : Traders divide large orders into smaller slices to minimize market impact. Order book updates guide the timing and size of each slice.
     - **Smart Order Routing**      : Algorithms use real-time order book data to route slices to exchanges with optimal liquidity.
     - **Market Making**            : Market makers adjust their quotes based on order book dynamics. They provide liquidity by placing limit orders.
     - **Spread Capture**           : Traders exploit spreads (the difference between best bid and ask) by executing at favorable levels.
     - **Midpoint Execution**       : Some algorithms target the midpoint (average of best bid and ask) for execution.
     - **Replenishment Strategies** : After executing a slice, traders monitor the order book for replenishment opportunities.

Remember, order book updates are crucial for informed trading decisions and effective execution strategies.

## Sample order book update to understand how it works.
Focus is on tick-level order book data, which provides detailed information about bids and asks at different price levels.

### Level 1 (L1) Data:
   - L1 data represents the **best bid** (highest price a buyer is willing to pay) and **best ask** (lowest price a seller is willing to accept) for a trading pair.
   - It's commonly referred to as "quote" data and is accessed in real-time.
   - Traders receive updates on the best bid and ask prices, along with their corresponding order amounts.
   - For instance:
     - Best Bid: $8000 (100 shares)
     - Best Ask: $8001 (50 shares)

### Level 2 (L2) Data:
   - L2 data provides more granularity than L1.
   - It includes **all bids and asks** at various price levels in the order book.
   - Market makers place limit orders at different price levels, creating a depth of book.
   - For example:
     - Traders place bids at $8000, $7999, and $7998.
     - Aggregated L2 quote: [Price: $8000, Amount: 23.4] (sum of all orders at that level).
#### How **aggregation** works for Level 2 (L2) order book data:

##### Aggregating Bids and Asks:
  - In the L2 order book, bids represent buy orders, and asks represent sell orders.
  - Aggregation combines multiple orders at the same price level into a single entry.
  - Here's how it works:
    - **Bids (Buy Orders)**:
      - Suppose we have the following bids:
        - Bid 1: Price = $8000, Amount = 10 shares
        - Bid 2: Price = $7999, Amount = 15 shares
        - Bid 3: Price = $7998, Amount = 8 shares
      - Aggregated bid: [Price: $8000, Amount: 33 shares] (sum of all bids at that level).
    - **Asks (Sell Orders)**:
      - Similarly, for asks:
        - Ask 1: Price = $8001, Amount = 20 shares
        - Ask 2: Price = $8002, Amount = 12 shares
        - Ask 3: Price = $8003, Amount = 5 shares
      - Aggregated ask: [Price: $8001, Amount: 37 shares] (sum of all asks at that level).

##### Weighted Aggregation:
   - The aggregation process considers both price and order size.
   - The aggregated quote reflects the total volume available at a specific price level.
   - For example:
     - If there are multiple bids at $8000, the aggregated bid amount is the sum of all individual bid amounts.
     - The price remains the same, but the total volume (amount) is combined.

##### Visualization:
   - Imagine the order book as a stack of price levels.
   - Each level aggregates orders at the same price.
   - Traders use this information to assess market depth and liquidity.

Remember, aggregation simplifies the order book representation while preserving essential information for traders.

### Level 3 (L3) Data:
   - L3 data goes even deeper.
   - It includes **individual bids and asks** placed by market makers.
   - Unlike L2 (aggregated by price level), L3 provides data for each individual order.
   - For the same example:
     - L3 data would show 10 different data points, each with an individual order amount.
     - More granular and used by sophisticated traders.

In summary:
- L1: Best bid/ask (quote) data.
- L2: Depth of book (market by price level) data.
- L3: Non-aggregated individual bids/asks (market by order) data.

Remember, each level provides increased granularity, catering to different trading strategies and needs¹.

#### Level 3 (L3) data is significantly more detailed and granular compared to Level 2 (L2) data. Let's explore why:

##### Individual Order Details:
   - L3 data captures **each individual order** placed by market participants.
   - For every bid or ask, it includes:
     - Price
     - Order size (number of shares or contracts)
     - Trader ID (anonymized)
     - Timestamp (when the order was placed)

##### Size of L3 Data:
   - L3 data can be substantial, especially in highly liquid markets.
   - Consider a stock with thousands of active traders and frequent order updates.
   - The data volume accumulates rapidly as orders flow in and out.
   - Real-time L3 updates can be quite large, especially during volatile periods.

##### Use Cases for L3 Data:
   - L3 data is essential for:
     - **High-Frequency Trading (HFT)** : HFT algorithms thrive on millisecond-level updates. They need precise order details.
     - **Market Making**                : Market makers adjust quotes based on individual orders.
     - **Arbitrage Strategies**         : Detecting price discrepancies across exchanges.
     - **Risk Management**              : Monitoring large orders and potential impact.
     - **Order Flow Analysis**          : Understanding trader behavior.

##### Sophistication and Complexity:
   - L3 data requires sophisticated infrastructure and processing capabilities.
   - Analyzing individual orders demands efficient algorithms and low-latency systems.
   - It's not always necessary for all traders; L2 data suffices for many.

In summary, while L3 data provides unparalleled granularity, it comes with computational challenges and is mainly used by specialized trading firms and professionals.



For more detailed information, you can explore resources like Kraken's WebSocket API documentation² and academic materials on order book algorithms⁵.

Source: Conversation with Bing, 2/28/2024
(1) WebSocket API v1 - How to maintain a valid order book | Kraken. https://support.kraken.com/hc/en-us/articles/360027821131-WebSocket-API-v1-How-to-maintain-a-valid-order-book.
(2) A Guided Tour of Chapter 8: Order Book Algorithms - Stanford University. https://web.stanford.edu/~ashlearn/RLForFinanceBook/Tour-OrderBook.pdf.
(3) New Books | New Book Releases | Barnes & Noble®. https://www.barnesandnoble.com/b/books/_/N-1sZ29Z8q8.
(4) Real-time order book data - IssuerView | S&P Global. https://www.spglobal.com/marketintelligence/en/mi/products/real-time-access-to-graphical-demand-and-allocation-data-issuerView.html.
(5) What is an order book? | Coinbase. https://www.coinbase.com/learn/advanced-trading/what-is-an-order-book.
(6) FX execution algorithms and market functioning - Bank for International .... https://www.bis.org/publ/mktc13.pdf.
(7) Thin Slicing: How to Make Smart Decisions, Fast | Shortform Books. https://www.shortform.com/blog/thin-slicing-malcolm-gladwell-blink/.

