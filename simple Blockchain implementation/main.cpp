/*
  Simple Blockchain Implementation (main.cpp)
  ---------------------------------------------
  - Complete blockchain: block struct, hashing, proof-of-work mining, basic transactions
  - CLI simulation: create genesis, add blocks, validate chain
  - Demonstrates hashing, immutability, and tampering detection (C++17, portable)
*/

#include <iostream>
#include <vector>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <string>
#include <openssl/sha.h>   // Linux: install libssl-dev, Windows: OpenSSL binaries
using namespace std;

// SHA-256 hash (using OpenSSL) as hex string
string sha256(const string& s) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256_ctx;
    SHA256_Init(&sha256_ctx);
    SHA256_Update(&sha256_ctx, s.c_str(), s.size());
    SHA256_Final(hash, &sha256_ctx);
    stringstream ss;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    return ss.str();
}

// Transaction structure (demo: from, to, amount)
struct Transaction {
    string from, to;
    double amount;
    Transaction(const string& f, const string& t, double a)
        : from(f), to(t), amount(a) {}
    string toString() const {
        stringstream ss;
        ss << from << "->" << to << ":" << amount;
        return ss.str();
    }
};

// Block structure
struct Block {
    int index;
    time_t timestamp;
    vector<Transaction> txs;
    string prevHash;
    string hash;
    int nonce;

    Block(int idx, vector<Transaction> txs_, const string& prev)
        : index(idx), timestamp(time(nullptr)), txs(move(txs_)), prevHash(prev), hash(""), nonce(0) {}

    string calcHash() const {
        stringstream ss;
        ss << index << timestamp << prevHash << nonce;
        for(const auto& tx : txs) ss << tx.toString();
        return sha256(ss.str());
    }

    void mine(int difficulty) {
        string prefix(difficulty, '0');
        do {
            nonce++;
            hash = calcHash();
        } while (hash.substr(0, difficulty) != prefix);
    }
};

// Blockchain class
struct Blockchain {
    vector<Block> chain;
    int difficulty = 4;   // # of leading zeros required in hash

    Blockchain() {        // Genesis block
        vector<Transaction> genesisTx = {Transaction("network", "satoshi", 50)};
        Block genesis(0, genesisTx, "0");
        genesis.mine(difficulty);
        genesis.hash = genesis.calcHash();
        chain.push_back(genesis);
    }

    void addBlock(const vector<Transaction>& txs) {
        int nextIdx = chain.size();
        string prevHash = chain.back().hash;
        Block blk(nextIdx, txs, prevHash);
        cout << "Mining block " << nextIdx << "...\n";
        blk.mine(difficulty);
        chain.push_back(blk);
    }

    bool isValid() const {
        for(size_t i=1; i < chain.size(); ++i) {
            const Block& curr = chain[i];
            const Block& prev = chain[i-1];
            if(curr.hash != curr.calcHash()) {
                cout << "Block #" << i << " has invalid hash!\n";
                return false;
            }
            if(curr.prevHash != prev.hash) {
                cout << "Block #" << i << "'s prevHash is invalid!\n";
                return false;
            }
        }
        return true;
    }

    void print() const {
        cout << "\n--- Blockchain State ---\n";
        for(const auto& blk : chain) {
            cout << "Block #" << blk.index << "\n";
            cout << "  Timestamp: " << ctime(&blk.timestamp);
            cout << "  PrevHash:  " << blk.prevHash << "\n";
            cout << "  Hash:      " << blk.hash << "\n";
            cout << "  Nonce:     " << blk.nonce << "\n";
            cout << "  TXs:\n";
            for(const auto& tx : blk.txs)
                cout << "    " << tx.from << " --> " << tx.to << " : " << tx.amount << "\n";
            cout << "\n";
        }
    }
};

// CLI demo
int main() {
    Blockchain myChain;
    string cmd;
    cout << "===== Simple Blockchain Demo =====\n";
    cout << "Commands: add, tamper, view, validate, quit\n";
    while(true) {
        cout << "\n> ";
        cin >> cmd;
        if(cmd == "add") {
            vector<Transaction> txs;
            int n;
            cout << "How many transactions in this block? ";
            cin >> n;
            for(int i=0; i<n; ++i) {
                string from, to; double amt;
                cout << "TX#" << i+1 << " from: "; cin >> from;
                cout << "      to: "; cin >> to;
                cout << "      amount: "; cin >> amt;
                txs.emplace_back(from, to, amt);
            }
            myChain.addBlock(txs);
            cout << "Block added!\n";
        } else if (cmd == "view") {
            myChain.print();
        } else if (cmd == "validate") {
            cout << (myChain.isValid() ? "Blockchain is VALID." : "Blockchain is CORRUPT!") << "\n";
        } else if (cmd == "tamper") {
            size_t idx;
            cout << "Enter block index to tamper (not 0): ";
            cin >> idx;
            if (idx > 0 && idx < myChain.chain.size()) {
                cout << "Setting first TX amount to 99999\n";
                if (!myChain.chain[idx].txs.empty())
                    myChain.chain[idx].txs[0].amount = 99999;
                myChain.chain[idx].hash = myChain.chain[idx].calcHash();
            }
        } else if (cmd == "quit") break;
        else cout << "Unknown command.\n";
    }
    cout << "Goodbye!\n";
    return 0;
}
