#pragma once

#include <eosiolib/asset.hpp>
#include <eosiolib/eosio.hpp>

#include <string>
#include "models.hpp"

namespace taikai {
    using namespace eosio;

    TABLE challenge_stat {
         kai_address  ch_address;
         std::string  challenge_id;
         uint64_t     total_funded;
         uint64_t     total_allocated;
         uint64_t     total_backed;
         uint64_t     projects_dividends;
         uint64_t     projects_dividends_dist;
         uint64_t     backers_dividends;
         uint64_t     backers_dividends_dist;
         kai_address  primary_key() const { return ch_address; }
         EOSLIB_SERIALIZE( challenge_stat,(ch_address)(challenge_id)(total_funded)(total_allocated)(total_backed)(projects_dividends)(projects_dividends_dist)(backers_dividends)(backers_dividends_dist));
    };

    using challenge_stats = eosio::multi_index< "chstats"_n, challenge_stat>;

    class challenge_stats_m {

    public:
        challenge_stats_m(name receiver, name code):
            _stats{receiver,code.value}
        {}

        void on_fund(const name owner,
                     const std::string& ch_id,
                     const kai_address& address,
                     const kai_ammount& amount)
        {
            auto it = _stats.find(address);
            if ( it == _stats.end() ) {
                _stats.emplace( owner, [&]( auto& row ) {
                    row.ch_address = address;
                    row.challenge_id = ch_id;
                    row.total_funded = address;
                    row.total_allocated = 0;
                    row.total_backed = 0;
                    row.projects_dividends = 0;
                    row.backers_dividends = 0;
                    row.projects_dividends_dist = 0;
                    row.backers_dividends_dist = 0;
                });
            } else {
                _stats.modify(it, owner, [&]( auto& row ) {
                    row.total_funded+= amount;
                });
            }
        }

        void on_allocate(const name owner,
                         const kai_address& address,
                         const kai_ammount& amount)
        {
            auto it = _stats.find(address);
            eosio_assert(it != _stats.end(), "Challenge not found on stats");
            _stats.modify(it, owner, [&]( auto& row ) {
                row.total_allocated+= amount;
            });
        }

        void on_back(const name owner,
                     const kai_address& address,
                     const kai_ammount& amount)
        {
            auto it = _stats.find(address);
            eosio_assert(it != _stats.end(), "Challenge not found on stats");
            _stats.modify(it, owner, [&]( auto& row ) {
                row.total_backed+= amount;
            });
        }

        void on_close(const name owner,
                     const kai_address& address)
        {
            auto it = _stats.find(address);
            eosio_assert(it != _stats.end(), "Challenge not found on stats");
            _stats.modify(it, owner, [&]( auto& row ) {
                row.projects_dividends = it->total_backed * PROJECT_DIV_PERC;
                row.backers_dividends = it->total_backed * INVESTOR_DIV_PERC;
            });
        }

        void on_project_div(const name owner,
                     const kai_address& address,
                    const kai_ammount& amount)
        {
            auto it = _stats.find(address);
            eosio_assert(it != _stats.end(), "Challenge not found on stats");
            _stats.modify(it, owner, [&]( auto& row ) {
                row.projects_dividends_dist+= amount;
            });
        }


        void on_backer_div(const name owner,
                           const kai_address& address,
                          const kai_ammount& amount)
        {
            auto it = _stats.find(address);
            eosio_assert(it != _stats.end(), "Challenge not found on stats");
            _stats.modify(it, owner, [&]( auto& row ) {
                row.backers_dividends_dist+= amount;
            });
        }

        kai_ammount get_total_funded(kai_address& address) {
             auto it = _stats.find(address);
             eosio_assert(it != _stats.end(), "Challenge not found on stats");
             return it->total_funded;
        }

        kai_ammount get_total_allocated(kai_address& address) {
             auto it = _stats.find(address);
             eosio_assert(it != _stats.end(), "Challenge not found on stats");
             return it->total_allocated;
        }

        kai_ammount get_total_backed(kai_address& address) {
             auto it = _stats.find(address);
             eosio_assert(it != _stats.end(), "Challenge not found on stats");
             return it->total_backed;
        }

        kai_ammount get_projects_dividends(kai_address& address) {
             auto it = _stats.find(address);
             eosio_assert(it != _stats.end(), "Challenge not found on stats");
             return it->projects_dividends;
        }

        kai_ammount get_backers_dividends(kai_address& address) {
             auto it = _stats.find(address);
             eosio_assert(it != _stats.end(), "Challenge not found on stats");
             return it->backers_dividends;
        }

     private:
        challenge_stats _stats;
    };
}