#include <gtest/gtest.h>
#include "parser/command_parser.h"

TEST(CommandParserTest, ParseSet)
{
    Command command =
        CommandParser::parse("SET name Saksham");

    EXPECT_EQ(command.type, CommandType::SET);
    EXPECT_EQ(command.key, "name");
    EXPECT_EQ(command.value, "Saksham");
}

TEST(CommandParserTest, ParseSetValueWithSpaces)
{
    Command command =
        CommandParser::parse("SET name Saksham Surajwal");

    EXPECT_EQ(command.type, CommandType::SET);
    EXPECT_EQ(command.key, "name");
    EXPECT_EQ(command.value, "Saksham Surajwal");
}

TEST(CommandParserTest, ParseGet)
{
    Command command =
        CommandParser::parse("GET name");

    EXPECT_EQ(command.type, CommandType::GET);
    EXPECT_EQ(command.key, "name");
}

TEST(CommandParserTest, ParseRemove)
{
    Command command =
        CommandParser::parse("REMOVE name");

    EXPECT_EQ(command.type, CommandType::REMOVE);
    EXPECT_EQ(command.key, "name");
}

TEST(CommandParserTest, ParseExists)
{
    Command command =
        CommandParser::parse("EXISTS name");

    EXPECT_EQ(command.type, CommandType::EXISTS);
    EXPECT_EQ(command.key, "name");
}

TEST(CommandParserTest, ParseClear)
{
    Command command =
        CommandParser::parse("CLEAR");

    EXPECT_EQ(command.type, CommandType::CLEAR);
}

TEST(CommandParserTest, ParseSize)
{
    Command command =
        CommandParser::parse("SIZE");

    EXPECT_EQ(command.type, CommandType::SIZE);
}


// _________________TEST CASES FOR INVALID COMMANDS___________________

TEST(CommandParserTest, RejectUnknownCommand)
{
    EXPECT_THROW(
        CommandParser::parse("DELETE name"),
        std::invalid_argument
    );
}

TEST(CommandParserTest, RejectEmptyCommand)
{
    EXPECT_THROW(
        CommandParser::parse(""),
        std::invalid_argument
    );
}

TEST(CommandParserTest, RejectSetWithoutKey)
{
    EXPECT_THROW(
        CommandParser::parse("SET"),
        std::invalid_argument
    );
}

TEST(CommandParserTest, RejectGetWithoutKey)
{
    EXPECT_THROW(
        CommandParser::parse("GET"),
        std::invalid_argument
    );
}