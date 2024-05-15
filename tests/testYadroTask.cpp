#include "../InputParser.hpp"
#include "../Event.hpp"
#include "../InvalidFormatException.hpp"
#include "../TableScheduler.hpp"


#include <gtest/gtest.h>
using namespace YadroTask;

TEST(InputParserTestSuit, OnInvalidTableInputThrowsInvalidFormatException) {
    InputParser parser;

    std::string invalidNumberString("not_number");
    EXPECT_THROW(parser.DirectReadNumber(invalidNumberString), InvalidFormatException);

    std::string invalidTableNumber("-100500");
    EXPECT_THROW(parser.DirectReadNumber(invalidTableNumber), InvalidFormatException);

    std::string validNumber("100500");
    EXPECT_NO_THROW(parser.DirectReadNumber(validNumber));
    ASSERT_EQ(parser.DirectReadNumber(validNumber), 100500);
}

TEST(InputParserTestSuit, OnInvalidTimeInputThrowsInvalidFormatException) {
    InputParser parser;

    std::string invalidTimeString("12345 12:45");
    EXPECT_THROW(parser.DirectReadDayTime(invalidTimeString), InvalidFormatException);

    std::string tooBigHoursNumber("25:59 23:39");
    EXPECT_THROW(parser.DirectReadDayTime(tooBigHoursNumber), InvalidFormatException);

    std::string tooManySymbols("12:59fff 11:23");
    EXPECT_THROW(parser.DirectReadDayTime(tooManySymbols), InvalidFormatException);

    std::string validTableNumber("10:30 20:00");
    std::vector<int> expected = {10, 30, 20, 0};
    EXPECT_NO_THROW(parser.DirectReadDayTime(validTableNumber));
    EXPECT_EQ(parser.DirectReadDayTime(validTableNumber), expected);
}

TEST(InputParserTestSuit, OnInvalidEventInputThrowsInvalidFormatException) {
    InputParser parser;

    std::string invalidId("12:00 404 Client");
    EXPECT_THROW(parser.DirectReadEvent(invalidId), InvalidFormatException);

    std::string invalidName("12:01 1 Client#");
    EXPECT_THROW(parser.DirectReadEvent(invalidName), InvalidFormatException);

    std::string tableNotSpecifiedForId2Event("13:43 2 Client");
    EXPECT_THROW(parser.DirectReadEvent(tableNotSpecifiedForId2Event), InvalidFormatException);

    std::string clientNameNotSpecified("14:43 1");
    EXPECT_THROW(parser.DirectReadEvent(clientNameNotSpecified), InvalidFormatException);

    std::string eventIsEarlierThanPreviousScanned("12:43 1 Client");
    EXPECT_THROW(parser.DirectReadEvent(eventIsEarlierThanPreviousScanned), InvalidFormatException);
}

TEST(TableSchedulerTestSuit, ClientComeEarlyTest) {
    TableScheduler clubWith2Tables100RubPerHourWorkingFrom8to20(2, 8, 0, 20, 0, 100);
    Event clientComeEarly(60, 1, "Client");
    clubWith2Tables100RubPerHourWorkingFrom8to20.ProcessEvent(clientComeEarly);
    clubWith2Tables100RubPerHourWorkingFrom8to20.CloseClub();
    ASSERT_EQ(clubWith2Tables100RubPerHourWorkingFrom8to20.getReport(), "08:00\n"
                                                                        "01:00 1 Client\n"
                                                                        "01:00 13 NotOpenYet\n"
                                                                        "20:00\n"
                                                                        "1 0 00:00\n"
                                                                        "2 0 00:00\n");
}

TEST(TableSchedulerTestSuit, ClientAlreadyInTest) {
    TableScheduler clubWith2Tables100RubPerHourWorkingFrom8to20(2, 8, 0, 20, 0, 100);
    Event clientCome(600, 1, "Client");
    Event clientComeAgain(620, 1, "Client");
    clubWith2Tables100RubPerHourWorkingFrom8to20.ProcessEvent(clientCome);
    clubWith2Tables100RubPerHourWorkingFrom8to20.ProcessEvent(clientComeAgain);
    clubWith2Tables100RubPerHourWorkingFrom8to20.CloseClub();
    ASSERT_EQ(clubWith2Tables100RubPerHourWorkingFrom8to20.getReport(), "08:00\n"
                                                                        "10:00 1 Client\n"
                                                                        "10:20 1 Client\n"
                                                                        "10:20 13 YouShallNotPass\n"
                                                                        "20:00\n"
                                                                        "1 0 00:00\n"
                                                                        "2 0 00:00\n");
}

TEST(TableSchedulerTestSuit, ClientUnknownTest) {
    TableScheduler clubWith2Tables100RubPerHourWorkingFrom8to20(2, 8, 0, 20, 0, 100);
    Event clientCome(600, 1, "Client");
    Event clientSeat(600, 2, "Client", 1);
    Event UnknownClientSeat(620, 2, "Client2", 2);
    Event UnknownClientExit(630, 4, "Client2");
    clubWith2Tables100RubPerHourWorkingFrom8to20.ProcessEvent(clientCome);
    clubWith2Tables100RubPerHourWorkingFrom8to20.ProcessEvent(clientSeat);
    clubWith2Tables100RubPerHourWorkingFrom8to20.ProcessEvent(UnknownClientSeat);
    clubWith2Tables100RubPerHourWorkingFrom8to20.ProcessEvent(UnknownClientExit);
    clubWith2Tables100RubPerHourWorkingFrom8to20.CloseClub();
    ASSERT_EQ(clubWith2Tables100RubPerHourWorkingFrom8to20.getReport(), "08:00\n"
                                                                        "10:00 1 Client\n"
                                                                        "10:00 2 Client 1\n"
                                                                        "10:20 2 Client2 2\n"
                                                                        "10:20 13 ClientUnknown\n"
                                                                        "10:30 4 Client2\n"
                                                                        "10:30 13 ClientUnknown\n"
                                                                        "20:00 11 Client\n"
                                                                        "20:00\n"
                                                                        "1 1000 10:00\n"
                                                                        "2 0 00:00\n");
}

TEST(TableSchedulerTestSuit, PlaceIsBusyTest) {
    TableScheduler clubWith2Tables100RubPerHourWorkingFrom8to20(2, 8, 0, 20, 0, 100);
    Event clientCome(600, 1, "Client");
    Event clientSeat(600, 2, "Client", 1);
    Event otherClientCome(600, 1, "Client2");
    Event otherClientSeat(620, 2, "Client2", 1);
    clubWith2Tables100RubPerHourWorkingFrom8to20.ProcessEvent(clientCome);
    clubWith2Tables100RubPerHourWorkingFrom8to20.ProcessEvent(clientSeat);
    clubWith2Tables100RubPerHourWorkingFrom8to20.ProcessEvent(otherClientCome);
    clubWith2Tables100RubPerHourWorkingFrom8to20.ProcessEvent(otherClientSeat);
    clubWith2Tables100RubPerHourWorkingFrom8to20.CloseClub();
    ASSERT_EQ(clubWith2Tables100RubPerHourWorkingFrom8to20.getReport(), "08:00\n"
                                                                        "10:00 1 Client\n"
                                                                        "10:00 2 Client 1\n"
                                                                        "10:00 1 Client2\n"
                                                                        "10:20 2 Client2 1\n"
                                                                        "10:20 13 PlaceIsBusy\n"
                                                                        "20:00 11 Client\n"
                                                                        "20:00\n"
                                                                        "1 1000 10:00\n"
                                                                        "2 0 00:00\n");
}