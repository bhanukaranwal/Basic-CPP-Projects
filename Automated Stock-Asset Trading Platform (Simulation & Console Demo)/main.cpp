/*
  Automated Asset Trading Platform (Simulation)
  -----------------------------------------------
  - Simulates market data for a fictional asset (e.g., ACME stock).
  - Allows "strategies" (simple moving average crossover demo included).
  - Executes simulated trades, logs portfolio performance.
  - Console-based, single-file, C++17 — no dependencies.
  - Illustrates algorithmic trading mechanics (but does NOT connect to real markets!).
*/

#include <iostream>
#include <vector>
#include <deque>
#include <random>
#include <chrono>
#include <iomanip>
#include <string>
#include <algorithm>
using namespace std;

// ---- Market Simulator ----
class Market {
    double price;
    mt19937 rng;
    normal_distribution<double> dist;
public:
    Market(double start) : price(start), rng(time(nullptr)), dist(0.0,1.0) {}
    // Simulate a random walk with a gentle uptrend
    double tick() {
        double drift = 0.02; // uptrend bias
        price *= (1.0 + drift/100.0 + dist(rng)/200.0);
        price = max(1.0, price); // no negative price!
        return price;
    }
};

// ---- Portfolio ----
struct Trade {
    int step;
    string side; // "BUY"/"SELL"
    double price;
    int qty;
};

struct Portfolio {
    double cash = 10000.0;
    int position = 0;
    double avgEntry = 0.0;
    vector<Trade> trades;
    void buy(int step, double price, int qty) {
        double cost = price*qty;
        if (cash >= cost && qty > 0) {
            cash -= cost;
            avgEntry = (position*avgEntry + price*qty)/(position+qty);
            position += qty;
            trades.push_back({step,"BUY",price,qty});
            cout << "  [TRADE] Bought " << qty << " @ " << fixed << setprecision(2) << price << "\n";
        }
    }
    void sell(int step, double price, int qty) {
        if (position >= qty && qty > 0) {
            cash += price*qty;
            position -= qty;
            trades.push_back({step,"SELL",price,qty});
            cout << "  [TRADE] Sold " << qty << " @ " << fixed << setprecision(2) << price << "\n";
            if(position == 0) avgEntry = 0;
        }
    }
    double netWorth(double lastPrice) const {
        return cash + position*lastPrice;
    }
};

// ---- Strategy: Moving Average Crossover ----
pair<bool,bool> movingAverageCross(const deque<double>& prices, int shortW, int longW) {
    if (prices.size() < longW) return {false,false};
    double shortSum = 0, longSum = 0;
    for (int i=0; i<shortW; ++i) shortSum += prices[prices.size()-1-i];
    for (int i=0; i<longW; ++i) longSum  += prices[prices.size()-1-i];
    double shortMA = shortSum/shortW, longMA = longSum/longW;
    static bool prevAbove = false;
    bool currAbove = shortMA > longMA;
    bool signalBuy = (!prevAbove && currAbove);
    bool signalSell = (prevAbove && !currAbove);
    prevAbove = currAbove;
    return {signalBuy, signalSell};
}

// ---- Main ----
int main() {
    cout << "===== Automated Asset Trading Simulator =====\n";
    cout << "Asset: ACME | Starting balance: $10,000 | Sim 60 ticks (1 per sec)\n";
    Market market(100.0);
    Portfolio pf;
    deque<double> priceHistory; // for MAs
    int shortW = 4, longW = 10;
    
    // Simulation loop: mimics realtime, one tick/second
    for (int step=1; step<=60; ++step) {
        double px = market.tick();
        priceHistory.push_back(px);
        if (priceHistory.size() > longW) priceHistory.pop_front();

        cout << "\n[Tick " << step << "] Price: $" << fixed << setprecision(2) << px;
        cout << " | Holdings: " << pf.position << " | Cash: $" << pf.cash << "\n";

        auto [signalBuy, signalSell] = movingAverageCross(priceHistory, shortW, longW);

        // Execute strategy
        if (signalBuy && pf.cash >= px*10) { // Buy 10 if enough cash
            pf.buy(step, px, 10);
        } else if (signalSell && pf.position >= 10) { // Sell 10 if holding
            pf.sell(step, px, 10);
        }

        cout << "  [Info] ShortMA(" << shortW << "): ";
        if(priceHistory.size() >= shortW) {
            double sum=0; for(int i=0;i<shortW;++i) sum+=priceHistory[priceHistory.size()-1-i];
            cout << fixed << setprecision(2) << sum/shortW;
        } else cout << "-";
        cout << " | LongMA(" << longW << "): ";
        if(priceHistory.size() >= longW) {
            double sum=0; for(int i=0;i<longW;++i) sum+=priceHistory[priceHistory.size()-1-i];
            cout << fixed << setprecision(2) << sum/longW;
        } else cout << "-";
        cout << "\n";
        this_thread::sleep_for(chrono::milliseconds(700));
    }

    // Final liquidation
    if (pf.position > 0) {
        cout << "\n[End of simulation] Liquidating " << pf.position << " @ $" << fixed << setprecision(2) << priceHistory.back() << "\n";
        pf.sell(61, priceHistory.back(), pf.position);
    }

    // Report
    cout << "\n===== Trading Summary =====\n";
    cout << "Final Net Worth: $" << fixed << setprecision(2) << pf.netWorth(priceHistory.back()) << "\n";
    cout << "Number of trades: " << pf.trades.size() << "\n";
    int wins=0, losses=0;
    for(size_t i=0;i+1<pf.trades.size();i+=2){
        if(pf.trades[i].side=="BUY" && pf.trades[i+1].side=="SELL") {
            if(pf.trades[i+1].price > pf.trades[i].price) ++wins; else ++losses;
        }
    }
    cout << "Winning trades: " << wins << ", Losing trades: " << losses << "\n";
    cout << "\nTRADE LOG:\n";
    cout << setw(8) << "Step" << setw(8) << "Side" << setw(12) << "Price" << setw(8) << "Qty" << "\n";
    for(const auto& t: pf.trades)
        cout << setw(8) << t.step << setw(8) << t.side << setw(12) << t.price << setw(8) << t.qty << "\n";
    cout << "\nSimulation complete.\n";
    return 0;
}
