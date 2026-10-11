#include "include/PieceTable.h"
#include <algorithm>
#include <cassert>
#include <iterator>

PieceTable::PieceTable(std::string_view initialText)
    : original(initialText)
{
    if (!original.empty())
    {
        pieces.push_back({Buffer::Original,
                          0,
                          original.size()});
    }
}

void PieceTable::moveToPiece(std::list<Piece>::iterator &it, std::size_t &remaining)
{
    for (; it != pieces.end(); ++it)
    {
        if (remaining < it->length)
            break;
        remaining -= it->length;
    }
    if (it == pieces.end())
        assert(remaining == 0);
    else
        assert(remaining < it->length);
}

char PieceTable::charAt(std::size_t position)
{
    auto it = pieces.begin();

    std::size_t remaining = position;

    moveToPiece(it, remaining);

    assert(it != pieces.end());

    const std::string &buffer = (it->buffer == Buffer::Original) ? original : add;
    return buffer[it->start + remaining];
}

std::string PieceTable::getText(std::size_t start, std::size_t length)
{
    auto it = pieces.begin();

    std::string text = "";
    std::size_t remaining = start;

    moveToPiece(it, remaining);

    std::size_t offset = remaining;

    for (; it != pieces.end() && length > 0; ++it)
    {
        const std::string &buffer = (it->buffer == Buffer::Original) ? original : add;
        std::size_t available = it->length - offset;
        std::size_t take = std::min(available, length);

        for (std::size_t i = 0; i < take; ++i)
            text += buffer[it->start + offset + i];

        length -= take;
        offset = 0;
    }

    return text;
}

Cursor PieceTable::cursorAt(std::size_t position)
{
    auto it = pieces.begin();
    std::size_t remaining = position;

    moveToPiece(it, remaining);

    if (it != pieces.end())
        return {it, remaining};

    if (pieces.empty())
        return {pieces.end(), 0};

    auto last = std::prev(pieces.end());
    return {last, last->length};
}

void PieceTable::insertText(Cursor &cursor, std::string_view text)
{
    const std::size_t textLen = text.size();
    if (textLen == 0)
        return;

    const bool emptyDocumentCursor = (cursor.piece == pieces.end());

    std::size_t pieceLength = 0;
    const std::size_t offset = cursor.offset;

    if (emptyDocumentCursor)
    {
        assert(pieces.empty());
        assert(cursor.offset == 0);
    }
    else
    {
        pieceLength = cursor.piece->length;
        assert(offset <= pieceLength);
    }

    std::size_t addStart = add.size();
    add += text;

    Piece newPiece{
        Buffer::Add,
        addStart,
        textLen};

    std::list<Piece>::iterator insertedIt = pieces.end();

    if (emptyDocumentCursor)
    {
        insertedIt = pieces.insert(pieces.end(), newPiece);
    }

    else if (offset == 0)
        insertedIt = pieces.insert(cursor.piece, newPiece);

    else if (offset == pieceLength)
        insertedIt = pieces.insert(std::next(cursor.piece), newPiece);
    else
    {
        Piece oldPiece = *cursor.piece;
        Piece leftPiece{
            oldPiece.buffer,
            oldPiece.start,
            offset};
        Piece rightPiece{
            oldPiece.buffer,
            oldPiece.start + offset,
            oldPiece.length - offset};
        *cursor.piece = leftPiece;
        insertedIt = pieces.insert(std::next(cursor.piece), newPiece);
        pieces.insert(std::next(insertedIt), rightPiece);
    }
    cursor.piece = insertedIt;
    cursor.offset = textLen;
}

void PieceTable::deleteText(std::size_t start, std::size_t length)
{
    auto it = pieces.begin();
    std::size_t remaining = start;

    moveToPiece(it, remaining);

    std::size_t offset = remaining;

    for (; it != pieces.end() && length > 0;)
    {
        Piece currentPiece = *it;
        std::size_t available = it->length - offset;
        std::size_t take = std::min(available, length);

        if (offset > 0)
        {
            Piece leftPiece{
                currentPiece.buffer,
                currentPiece.start,
                offset};
            pieces.insert(it, leftPiece);
        }
        if (take < available)
        {
            Piece rightPiece{
                currentPiece.buffer,
                currentPiece.start + offset + take,
                available - take};
            pieces.insert(it, rightPiece);
        }
        it = pieces.erase(it);
        length -= take;
        offset = 0;
    }
}