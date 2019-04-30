/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */

#include "taikai.fund.hpp"
#include "models.hpp"
#include <eosiolib/multi_index.hpp>
#include <algorithm>

namespace taikai {

    kai_address getAddress(std::string address){
        kai_address result;
        std::string prefix = "KAI01";
        std::size_t pos = address.find(prefix);
        std::string value = address.substr(pos+prefix.size());
        eosio_assert( value.size() == 16 , address.c_str());
        result = std::stoull (value,nullptr,16);
        return result;
    }

    using namespace eosio;

    ACTION funding::mint(uint64_t  treasury,
                         uint64_t  amount) {

        require_auth(get_self());
        eosio_assert( amount > 0, "Minted amount must be positive");
        // 1. Validate if the Symbol already exists
        auto existing = _treasuries.find(treasury);
        if ( existing != _treasuries.end()) {
             eosio::print("Incrementing Mint ",treasury," suply by ", amount, "\n");
            // Increment the supply
            _treasuries.modify(existing, get_self(), [&]( auto& row ) {
                row.amount+= amount;
                row.max_amount+= amount;
            });
        } else {
            eosio::print("Creating Mint ",treasury, " supply by ", amount, "\n");
             // Launch the supply
            _treasuries.emplace( get_self(), [&]( auto& obj ){
                obj.id = treasury;
                obj.amount = amount;
                obj.max_amount = amount;
            });
        }
    }

    ACTION funding::issue(uint64_t treasury,
                         std::string wallet,
                         uint64_t amount)
    {
        require_auth(get_self());
        eosio_assert( amount > 0, "issued amount must be positive");
        auto treasuryRow = _treasuries.find(treasury);
        eosio_assert( treasuryRow != _treasuries.end(), "Account not found to fund challenge");
        eosio_assert( treasuryRow->amount >= amount, "No Balance found on treasury");
        _treasuries.modify(treasuryRow, get_self(), [&]( auto& row ) {
                row.amount-= amount;
        });
        kai_address address{getAddress(wallet)};
        auto idx = _wallets.template get_index<"address"_n>();
        auto iterator = idx.find(address);
        if (iterator == idx.end() ) {
             _wallets.emplace(get_self(), [&]( auto& row ) {
                row.id = get_wallet_id();
                row.type = ACCOUNT;
                row.wallet_address = address;
                row.balance = amount;
            });
        } else {
             eosio_assert(iterator->type == ACCOUNT,
                "Destination wallet is not an account wallet");
             idx.modify(iterator, get_self(), [&]( auto& row ) {
                row.balance+= amount;
            });
        }

    }


    ACTION funding::fund( std::string wallet,
                          std::string challenge_id,
                          std::string ch_wallet,
                          uint64_t amount)
    {
        eosio::print("Funding KAIS with = ", wallet, "\n");
        require_auth(get_self());
        eosio_assert( amount > 0, "issued amount must be positive");

        //1 . Update Account Balance
        auto idx = _wallets.template get_index<"address"_n>();
        auto account = idx.find(getAddress(wallet));
        eosio_assert( account != idx.end(), "Fund not found");
        eosio_assert( account->balance >= amount, "No balance to finance challenge");
        idx.modify(account, get_self(), [&]( auto& row ) {
                row.balance-= amount;
        });
        kai_address address{getAddress(ch_wallet)};
        auto iterator = idx.find(address);
        if (iterator == idx.end() ) {
            _wallets.emplace(get_self(), [&]( auto& row ) {
                row.id = get_wallet_id();
                row.type = CHALLENGE;
                row.challenge_id = challenge_id;
                row.wallet_address = address;
                row.balance = amount;
            });
        } else {
             eosio_assert(iterator->type == CHALLENGE,
                "Destination wallet is not a challenge wallet");
             idx.modify(iterator, get_self(), [&]( auto& row ) {
                row.balance+= amount;
            });
        }
        _stats.on_fund(get_self(), challenge_id, address, amount);
    }

    ACTION funding::allocate(std::string ch_wallet,
                        std::string user_wallet,
                        uint64_t amount)
    {
        require_auth(get_self());
        // 1. Find the challenge Wallet
         auto idx = _wallets.template get_index<"address"_n>();
         kai_address address(getAddress(ch_wallet));
         auto iterator = idx.find(address);
         eosio_assert( iterator != idx.end(), "Challenge wallet not found");
         eosio_assert( iterator->balance >= amount, "Not enought balance on challenge wallet");
         eosio_assert( iterator->type == CHALLENGE,
                "Source wallet is not a challenge wallet");
         // 2. Grab the balance from challenge wallet
         idx.modify(iterator, get_self(), [&]( auto& row ) {
                row.balance-= amount;
        });

         auto userIdx = _wallets.template get_index<"address"_n>();
         kai_address userAddress{getAddress(user_wallet)};
         auto userIterator = userIdx.find(userAddress);
        // 3. Update the user wallet related to the challenge
        if (userIterator == userIdx.end() ) {
            _wallets.emplace(get_self(), [&]( auto& row ) {
                row.id = get_wallet_id();
                row.type = CHALLENGE_USER;
                row.parent_address = address;
                row.wallet_address = userAddress;
                row.balance = amount;
            });
        } else {
            eosio::print("Wallet updated \n");
            eosio_assert( userIterator->type == CHALLENGE_USER,
                "Destination wallet is not a user challenge wallet");
            eosio::print("issuing Kais to Challenge wallet = ", ch_wallet, "\n");
             userIdx.modify(userIterator, get_self(), [&]( auto& row ) {
                row.balance+= amount;
            });
        }
        _stats.on_allocate(get_self(), address, amount);
    }

    ACTION funding::transfer(std::string ch_wallet,
                          std::string from_wallet,
                          std::string to_wallet,
                          uint64_t amount)
    {
        require_auth(get_self());
        // 1. Find the challenge Wallet
        auto idx = _wallets.template get_index<"address"_n>();
        kai_address chWalletAdx(getAddress(ch_wallet));
        auto iterator = idx.find(chWalletAdx);
        eosio_assert( iterator != idx.end(), "Challenge wallet not found");

        auto userIdx = _wallets.template get_index<"address"_n>();
        kai_address fromWalletAdx(getAddress(from_wallet));
        kai_address toWalletAdx(getAddress(to_wallet));
        auto fromIterator = userIdx.find(fromWalletAdx);
        auto toIterator = userIdx.find(toWalletAdx);
        eosio_assert(fromIterator != userIdx.end(), "From wallet not found");
        eosio_assert(fromIterator->balance >= amount,"Not enough balance");
        eosio_assert(chWalletAdx==fromIterator->parent_address,"Parent wallets are not the same");
        eosio_assert( fromIterator->type == CHALLENGE_USER,
                "Source wallet is not a challenge user wallet");
        userIdx.modify(fromIterator, get_self(), [&]( auto& row ) {
                row.balance-= amount;
        });
        if ( toIterator == userIdx.end() ) {
             _wallets.emplace(get_self(), [&]( auto& row ) {
                row.id = get_wallet_id();
                row.type = CHALLENGE_USER;
                row.parent_address = chWalletAdx;
                row.wallet_address = toWalletAdx;
                row.balance= amount;
            });
        } else {
            eosio_assert(chWalletAdx==toIterator->parent_address,"Parent wallets are not the same");
            eosio_assert(toIterator->type == CHALLENGE_USER,
                "Destination wallet is not a user challenge wallet");
            userIdx.modify(toIterator, get_self(), [&]( auto& row ) {
                row.balance+= amount;
            });
        }
    }


    ACTION funding::back(std::string ch_wallet,
                      std::string user_wallet,
                      std::string project_wallet,
                      uint64_t amount)
    {
        require_auth(get_self());
        auto idx = _wallets.template get_index<"address"_n>();
        kai_address chWalletAdx(getAddress(ch_wallet));
        auto iterator = idx.find(chWalletAdx);
        eosio_assert( iterator != idx.end(), "Challenge wallet not found");

        auto userIdx = _wallets.template get_index<"address"_n>();
        kai_address fromWalletAdx(getAddress(user_wallet));
        auto fromIterator = userIdx.find(fromWalletAdx);
        eosio_assert(fromIterator != userIdx.end(), "From wallet not found");
        eosio_assert(fromIterator->balance >= amount,"Not enough balance");
        eosio_assert(chWalletAdx==fromIterator->parent_address,"Parent wallets are not the same");
        eosio_assert( fromIterator->type == CHALLENGE_USER,
                "Source wallet is not a challenge user wallet");
        userIdx.modify(fromIterator, get_self(), [&]( auto& row ) {
                row.balance-= amount;
        });
        kai_address toWalletAdx(getAddress(project_wallet));
        auto projectIdx = _wallets.template get_index<"address"_n>();
        auto toIterator = projectIdx.find(toWalletAdx);
        if ( toIterator == projectIdx.end() ) {
            _wallets.emplace(get_self(), [&]( auto& row ) {
                row.id = get_wallet_id();
                row.type = PROJECT;
                row.parent_address = chWalletAdx;
                row.wallet_address = toWalletAdx;
                row.balance = amount;
            });
        } else {
            eosio_assert(chWalletAdx == toIterator->parent_address,"Parent wallets are not the same");
            eosio_assert( toIterator->type == PROJECT,
                "Destination wallet is not a project wallet");
            projectIdx.modify(toIterator, get_self(), [&]( auto& row ) {
                row.balance+= amount;
            });
        }
        handle_backer_update(chWalletAdx, fromWalletAdx, toWalletAdx, amount);
        _stats.on_back(get_self(), chWalletAdx, amount);
    }

    ACTION funding::close(std::string ch_wallet)
    {
        require_auth(get_self());
        auto idx = _wallets.template get_index<"address"_n>();
        kai_address chWalletAdx(getAddress(ch_wallet));
        print("Closing challenge investiment phase for ", ch_wallet.c_str());
        auto iterator = idx.find(chWalletAdx);
        eosio_assert( iterator != idx.end(), "Challenge wallet not found");
        _stats.on_close(get_self(),chWalletAdx);
        std::multiset<ch_balance,wallet_comparator> items;
        sort_project_balances(chWalletAdx,items);
        fill_final_balance(items);
    }



    ACTION funding::collectaward( std::string ch_wallet,
                                   std::string project_wallet,
                                   uint64_t amount)
    {
        require_auth(get_self());
        kai_address ch_wallet_adx(getAddress(ch_wallet));
        kai_address project_wallet_adx(getAddress(project_wallet));
        auto idx = _wallets.template get_index<"address"_n>();
        auto iterator = idx.find(project_wallet_adx);
        // Validate that the project wallet exists
        eosio_assert( iterator != idx.end(), "Project wallet not found");
        // Validate that the ammount collected is the right onr
        eosio_assert( iterator->balance == amount, "Invalid ammount to collect");
        credit_wallet(ch_wallet_adx, CHALLENGE_AWARD, amount);
        // Delete the Project Wallet
        idx.erase(iterator);
    }

    ACTION funding::divproject(std::string ch_wallet,
                                std::string project_wallet,
                                uint64_t amount)
    {
        require_auth(get_self());
        kai_address ch_wallet_adx(getAddress(ch_wallet));
        kai_address project_wallet_adx(getAddress(project_wallet));
        debit_wallet(ch_wallet_adx,CHALLENGE_AWARD, amount);
        credit_wallet(project_wallet_adx,PROJECT_DIVIDEND, amount);
        _stats.on_project_div(get_self(), ch_wallet_adx, amount);
        _dstats.on_project_dividend(get_self(),ch_wallet_adx,project_wallet_adx,amount);
    }

    ACTION funding::divmember(std::string project_wallet,
                               std::string user_wallet,
                               uint64_t amount)
    {
         require_auth(get_self());
        kai_address project_wallet_adx(getAddress(project_wallet));
        kai_address user_wallet_adx(getAddress(user_wallet));
        debit_wallet(project_wallet_adx,PROJECT_DIVIDEND, amount);
        credit_wallet(user_wallet_adx, USER, amount);
        _dstats.on_member_dividend(get_self(),project_wallet_adx,user_wallet_adx,amount);
    }

    ACTION funding::divbacker(std::string ch_wallet,
                              std::string user_wallet,
                              uint64_t amount)
    {
        require_auth(get_self());
        kai_address ch_wallet_adx(getAddress(ch_wallet));
        kai_address user_wallet_adx(getAddress(user_wallet));
        debit_wallet(ch_wallet_adx,CHALLENGE_AWARD, amount);
        credit_wallet(user_wallet_adx, USER, amount);
        _stats.on_backer_div(get_self(), ch_wallet_adx, amount);
        _dstats.on_backer_dividend(get_self(),ch_wallet_adx,user_wallet_adx,amount);
    }


    ACTION funding::collectlefts(std::string  user_wallet,
                                 std::string  account_wallet,
                                 uint64_t    amount)
    {
        require_auth(get_self());
        auto userIdx = _wallets.template get_index<"address"_n>();
        kai_address from_wallet_adx(getAddress(user_wallet));
        print("Collecting ",amount," from ",from_wallet_adx,"\n");
        kai_address to_wallet_adx(getAddress(account_wallet));
        auto fromIterator = userIdx.find(from_wallet_adx);
        auto toIterator = userIdx.find(to_wallet_adx);
        eosio_assert(fromIterator != userIdx.end(), "From wallet not found");
        eosio_assert(fromIterator->balance == amount, "Invalid balance to collect");
        eosio_assert(fromIterator->type == CHALLENGE_USER,
                "Source wallet is not a challenge user wallet");
        debit_wallet(from_wallet_adx,CHALLENGE_USER, amount);
        credit_wallet(to_wallet_adx, ACCOUNT, amount);
        userIdx.erase(fromIterator);
    }

    void funding::handle_backer_update(
        const kai_address& challenge_adx,
        const kai_address& investor_adx,
        const kai_address& project_adx,
        const uint64_t& amount)
    {
        auto backerIdx = _backers.template get_index<"backer"_n>();
        uint128_t key = compoundKey<uint128_t, uint64_t,uint64_t>(investor_adx, project_adx);
        auto backerIter = backerIdx.find(key);
        if ( backerIter == backerIdx.end() ) {
            _backers.emplace(get_self(), [&]( auto& row ) {
                row.id = get_backer_id();
                row.parent_address = challenge_adx;
                row.project_address = project_adx;
                row.investor_address = investor_adx;
                row.amount = amount;
            });
        } else {
             backerIdx.modify(backerIter, get_self(), [&]( auto& row ) {
                row.amount+= amount;
            });
        }
    }

    uint64_t funding::get_wallet_id() const {
        auto size = std::distance(_wallets.cbegin(),_wallets.cend());;
        return size > 0 ? (--_wallets.end())->id+1 : 1;
    }

    uint64_t funding::get_backer_id() const {
        auto size = std::distance(_backers.cbegin(),_backers.cend());;
        return size > 0 ? (--_backers.end())->id+1 : 1;
    }

    void funding::clean_challenge_wallets(kai_address &wallet) {
        auto index = _wallets.template get_index<name("parent")>();
        for ( auto it = index.find(wallet);
              it != index.end();
              it = index.find(wallet))
        {
            index.erase(it);
        }
    }

    void funding::sort_project_balances(
        const kai_address& project_adx, ch_balance_set& balance_set)
    {
        auto parentIdx = _wallets.template get_index<"parent"_n>();
        for(auto it = parentIdx.find(project_adx);
            it != parentIdx.end() && it->parent_address == project_adx  ; it++ )
        {
            ch_balance item;
            if ( it-> type == PROJECT ){
                item.parent_address = it->parent_address;
                item.project_address = it->wallet_address;
                item.balance = it->balance;
                balance_set.insert(item);
            }
        }
    }

    void funding::fill_final_balance(const ch_balance_set& bset){
        uint16_t position = 0;
        uint64_t prevScore = 0;
        uint16_t projectDividendCount = 0;

        for ( auto& item : bset) {
            uint64_t dividend = 0;
            std::decay<decltype(item)&>::type updateItem = item;
            if ( prevScore != item.balance ) {
                updateItem.position = ++position;
                prevScore = item.balance;
            } else {
                updateItem.position = position;
            }
            _ch_balances.emplace(get_self(), [&]( auto& row ) {
                row.position = updateItem.position;
                row.parent_address = updateItem.parent_address;
                row.project_address = updateItem.project_address;;
                row.balance = updateItem.balance;
            });
            print(" Project ",updateItem.project_address," position",updateItem.position," ",updateItem.balance);
        };
    }


    void funding::credit_wallet(kai_address address, WalletType type, uint64_t amount)
    {
        auto chant_idx = _wallets.template get_index<"chandtype"_n>();
        uint128_t key = compoundKey<uint128_t,uint64_t,uint64_t>(address, type);
        auto it = chant_idx.find(key);
        if ( it == chant_idx.end() ) {
            print("Credit new wallet with key",key);
            _wallets.emplace(get_self(), [&]( auto& row ) {
                    row.id = get_wallet_id();
                    row.type = type;
                    row.wallet_address = address;
                    row.balance = amount;
            });
        } else {
            eosio_assert(it->type == type, "Invalid type");
            print("Credit a wallet with key",key);
            chant_idx.modify(it, get_self(), [&]( auto& row ) {
                row.balance+= amount;
            });
        }
    }

    void funding::debit_wallet(kai_address address, WalletType type, uint64_t amount){
        auto chant_idx = _wallets.template get_index<"chandtype"_n>();
        uint128_t key = compoundKey<uint128_t,uint64_t,uint64_t>(address, type);
        auto award_wallet_it = chant_idx.find(key);
        eosio_assert( award_wallet_it != chant_idx.end(), "Award Wallet not found");
        eosio_assert( award_wallet_it->balance >= amount, "No balance on award wallet");
        eosio_assert( award_wallet_it->type == type, "Invalid type");
        chant_idx.modify(award_wallet_it, get_self(), [&]( auto& row ) {
                row.balance-= amount;
        });
    }

} /// namespace eosio

EOSIO_DISPATCH(taikai::funding,(mint)(issue)(fund)(allocate)(transfer)(back)(close)(collectaward)(divproject)(divbacker)(divmember)(collectlefts))
