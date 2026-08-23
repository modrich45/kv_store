#include <gtest/gtest.h>
#include "kv/command_parser.h"

TEST(CommandParserTest, ParseSetCommand) {
    CommandParser parser;
    Command cmd = parser.parse("SET name Vishal");

    EXPECT_EQ(cmd.type, CommandType::SET);
    EXPECT_EQ(cmd.key, "name");
    EXPECT_EQ(cmd.value, "Vishal");
}

TEST(CommandParserTest, ParseGetCommand) {
    CommandParser parser;
    Command cmd = parser.parse("GET name");

    EXPECT_EQ(cmd.type, CommandType::GET);
    EXPECT_EQ(cmd.key, "name");
    EXPECT_EQ(cmd.value, "");
}

TEST(CommandParserTest, ParseDeleteCommand) {
    CommandParser parser;
    Command cmd = parser.parse("REMOVE name");

    EXPECT_EQ(cmd.type, CommandType::REMOVE);
    EXPECT_EQ(cmd.key, "name");
    EXPECT_EQ(cmd.value, "");
}

TEST(CommandParserTest, ParseInvalidCommand) {
    CommandParser parser;
    Command cmd = parser.parse("INVALID name");

    EXPECT_EQ(cmd.type, CommandType::INVALID);
    EXPECT_EQ(cmd.key, "");
    EXPECT_EQ(cmd.value, "");
}

TEST(CommandParserTest, ExistsCommand) {
    CommandParser parser;
    Command cmd = parser.parse("EXISTS name");

    EXPECT_EQ(cmd.type, CommandType::EXISTS);
    EXPECT_EQ(cmd.key, "name");
    EXPECT_EQ(cmd.value, "");
}

TEST(CommandParserTest, SizeCommand) {
    CommandParser parser;
    Command cmd = parser.parse("SIZE");

    EXPECT_EQ(cmd.type, CommandType::SIZE);
    EXPECT_EQ(cmd.key, "");
    EXPECT_EQ(cmd.value, "");
}

TEST(CommandParserTest, ClearCommand) {
    CommandParser parser;
    Command cmd = parser.parse("CLEAR");

    EXPECT_EQ(cmd.type, CommandType::CLEAR);
    EXPECT_EQ(cmd.key, "");
    EXPECT_EQ(cmd.value, "");
}

