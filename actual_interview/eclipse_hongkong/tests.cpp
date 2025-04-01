#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include <fakeit.hpp>
#include "InstrumentQuoter.hpp"

using namespace fakeit;

TEST_CASE("InstrumentQuoter initialization") {
    Mock<Execution> mockExecution;
    
    SECTION("Valid initialization") {
        REQUIRE_NOTHROW(InstrumentQuoter("AAPL", 1.0, 100, 0.01, mockExecution.get()));
    }
    
    SECTION("Invalid tick width") {
        REQUIRE_THROWS_AS(InstrumentQuoter("AAPL", 1.0, 100, 0.0, mockExecution.get()), 
                         std::invalid_argument);
    }
}

TEST_CASE("Price calculation") {
    Mock<Execution> mockExecution;
    InstrumentQuoter quoter("AAPL", 1.0, 100, 0.5, mockExecution.get());

    SECTION("Buy price with existing bid") {
        quoter.OnTheoreticalPrice(100.0);
        quoter.OnBestBidOffer(99.5, 100.5);
        REQUIRE(InstrumentQuoterTestAccess::calculatePrice(quoter, 'B') == 99.0);
    }

    SECTION("Negative prices") {
        quoter.OnTheoreticalPrice(-10.0);
        quoter.OnBestBidOffer(-11.0, -9.0);
        REQUIRE(InstrumentQuoterTestAccess::calculatePrice(quoter, 'B') == -11.5);
        REQUIRE(InstrumentQuoterTestAccess::calculatePrice(quoter, 'S') == -8.5);
    }
}

TEST_CASE("Order management") {
    Mock<Execution> mockExecution;
    When(Method(mockExecution, requestOrderAdd)).AlwaysReturn();

    InstrumentQuoter quoter("AAPL", 1.0, 100, 0.5, mockExecution.get());

    SECTION("New quotes when no orders exist") {
        quoter.OnTheoreticalPrice(100.0);
        quoter.OnBestBidOffer(99.5, 100.5);
        Verify(Method(mockExecution, requestOrderAdd)).Exactly(2);
    }

    SECTION("Order confirmation flow") {
        quoter.OnTheoreticalPrice(100.0);
        quoter.OnBestBidOffer(99.5, 100.5);
        quoter.OnOrderAddConfirm(1);
        REQUIRE_FALSE(InstrumentQuoterTestAccess::shouldSendQuote(quoter, 'B'));
    }
}

TEST_CASE("Validation checks") {
    Mock<Execution> mockExecution;
    InstrumentQuoter quoter("AAPL", 1.0, 100, 0.5, mockExecution.get());
    quoter.OnTheoreticalPrice(100.0);
    quoter.OnBestBidOffer(99.5, 100.5);

    InstrumentQuoter::Quote validBuyOrder('B', 99.0, 100);
    InstrumentQuoter::Quote invalidBuyOrder('B', 99.6, 100);

    SECTION("Valid offset check") {
        REQUIRE(InstrumentQuoterTestAccess::validOffset(quoter, validBuyOrder));
        REQUIRE_FALSE(InstrumentQuoterTestAccess::validOffset(quoter, invalidBuyOrder));
    }

    SECTION("Not crossing check") {
        REQUIRE(InstrumentQuoterTestAccess::notCrossing(quoter, validBuyOrder));
    }
}

int main(int argc, char* argv[]) {
    return Catch::Session().run(argc, argv);
}
