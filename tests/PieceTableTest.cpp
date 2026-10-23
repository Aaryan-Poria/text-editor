#include "PieceTable.h"
#include <gtest/gtest.h>

// Construction Tests

TEST(PieceTableTest, InitialTableConstruction)
{
    PieceTable table("hello");
    EXPECT_EQ(table.getText(0, 5), "hello");
}

// Insertion Tests

TEST(PieceTableTest, EmptyDocumentInsertion)
{
    PieceTable table;
    Cursor cursor = table.cursorAt(0);

    table.insertText(cursor, "hello");

    EXPECT_EQ(table.getText(0, 5), "hello");
}

TEST(PieceTableTest, InsertAtBeginning)
{
    PieceTable table("world");
    Cursor cursor = table.cursorAt(0);

    table.insertText(cursor, "hello ");

    EXPECT_EQ(table.getText(0, 11), "hello world");
}

TEST(PieceTableTest, InsertInMiddle)
{
    PieceTable table("ac");
    Cursor cursor = table.cursorAt(1);

    table.insertText(cursor, "b");

    EXPECT_EQ(table.getText(0, 3), "abc");
}

TEST(PieceTableTest, InsertAtEnd)
{
    PieceTable table("hello");
    Cursor cursor = table.cursorAt(5);

    table.insertText(cursor, " world");

    EXPECT_EQ(table.getText(0, 11), "hello world");
}

TEST(PieceTableTest, EmptyInsertionDoesNotChangeDocument)
{
    PieceTable table("hello");
    Cursor cursor = table.cursorAt(2);

    table.insertText(cursor, "");

    EXPECT_EQ(table.getText(0, 5), "hello");
}

TEST(PieceTableTest, CursorAdvancesAfterInsertion)
{
    PieceTable table("ab");
    Cursor cursor = table.cursorAt(1);

    table.insertText(cursor, "XY");
    table.insertText(cursor, "Z");

    EXPECT_EQ(table.getText(0, 5), "aXYZb");
}

// Reading Tests

TEST(PieceTableTest, CharAtReadsAcrossPieces)
{
    PieceTable table("ac");
    Cursor cursor = table.cursorAt(1);

    table.insertText(cursor, "b");

    EXPECT_EQ(table.charAt(0), 'a');
    EXPECT_EQ(table.charAt(1), 'b');
    EXPECT_EQ(table.charAt(2), 'c');
}

TEST(PieceTableTest, GetTextReadsAcrossPieceBoundaries)
{
    PieceTable table("ac");
    Cursor cursor = table.cursorAt(1);

    table.insertText(cursor, "b");

    EXPECT_EQ(table.getText(0, 3), "abc");
    EXPECT_EQ(table.getText(1, 2), "bc");
}

TEST(PieceTableTest, GetTextClipsAtDocumentEnd)
{
    PieceTable table("hello");

    EXPECT_EQ(table.getText(3, 100), "lo");
    EXPECT_EQ(table.getText(5, 100), "");
}

TEST(PieceTableTest, GetTextWithZeroLengthReturnsEmpty)
{
    PieceTable table("hello");

    EXPECT_EQ(table.getText(2, 0), "");
}

// Deletion Tests

TEST(PieceTableTest, DeleteWithinPiece)
{
    PieceTable table("abcdef");

    table.deleteText(2, 2);

    EXPECT_EQ(table.getText(0, 4), "abef");
}

TEST(PieceTableTest, DeleteAcrossPieceBoundaries)
{
    PieceTable table("ace");
    Cursor cursor = table.cursorAt(1);

    table.insertText(cursor, "bd");
    table.deleteText(1, 3);

    EXPECT_EQ(table.getText(0, 2), "ae");
}

TEST(PieceTableTest, DeleteClipsAtDocumentEnd)
{
    PieceTable table("abcdef");

    table.deleteText(3, 100);

    EXPECT_EQ(table.getText(0, 3), "abc");
}

TEST(PieceTableTest, DeleteWithZeroLengthDoesNothing)
{
    PieceTable table("hello");

    table.deleteText(2, 0);

    EXPECT_EQ(table.getText(0, 5), "hello");
}

TEST(PieceTableTest, DeleteStartingAtDocumentEndDoesNothing)
{
    PieceTable table("hello");

    table.deleteText(5, 100);

    EXPECT_EQ(table.getText(0, 5), "hello");
}

TEST(PieceTableTest, CanInsertAfterDeletingEntireDocument)
{
    PieceTable table("abc");

    table.deleteText(0, 3);
    EXPECT_EQ(table.getText(0, 1), "");

    Cursor cursor = table.cursorAt(0);
    table.insertText(cursor, "x");

    EXPECT_EQ(table.getText(0, 1), "x");
}