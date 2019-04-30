#include <iostream>
#include "gtest/gtest.h"
#include "prisma_id.hpp"
#include <string>

using namespace taikai;

TEST(KeyGeneration, MinuteInterval) {
	std::string id{"cjqc6jklqtran0a98pcxn9h6c"};
	prisma_id pId{id};
	EXPECT_EQ((pId.get_value()| static_cast<eosio::uint128_t>(0x1f), 10);
}