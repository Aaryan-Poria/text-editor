#include <iostream>
#include <list>

enum class Buffer
{
    Original,
    Add
};

struct Piece
{
    Buffer buffer;
    size_t start;
    size_t length;
};

struct Cursor
{
    std::list<Piece>::iterator piece;
    size_t offset;
};

class PieceTable
{
private:
    std::string original;
    std::string add;
    std::list<Piece> pieces;

public:
    void moveToPosition(std::list<Piece>::iterator &it, size_t &remaining)
    {
        for (; it != pieces.end(); ++it)
        {
            if (remaining < it->length)
                break;
            remaining -= it->length;
        }
    }
    char charAt(size_t position)
    {
        size_t remaining = position;
        for (const auto &piece : pieces)
        {
            if (remaining < piece.length)
            {
                const std::string &buffer = (piece.buffer == Buffer::Original) ? original : add;
                return buffer[piece.start + remaining];
            }
            remaining -= piece.length;
        }
    }
    std::string getText(size_t start, size_t length)
    {
        std::string text;
        size_t remaining = start;

        auto it = pieces.begin();

        moveToPosition(it, remaining);

        size_t offset = remaining;

        for (; it != pieces.end() && length > 0; ++it)
        {
            const std::string &buffer = (it->buffer == Buffer::Original) ? original : add;
            size_t available = it->length - offset;
            size_t take = std::min(available, length);

            for (size_t i = 0; i < take; ++i)
                text += buffer[it->start + offset + i];

            length -= take;
            offset = 0;
        }
        return text;
    }

    void insertText(Cursor &cursor, std::string_view text)
    {
        const size_t textLength = text.size();
        if (textLength == 0)
            return;

        const size_t pieceLength = cursor.piece->length;
        const size_t offset = cursor.offset;

        size_t addStart = add.size();
        add += text;

        Piece insertedPiece{
            Buffer::Add,
            addStart,
            textLength};
        if (offset == 0)
        {
            auto insertedIt = pieces.insert(cursor.piece, insertedPiece);
            cursor.piece = insertedIt;
        }
        else if (offset == pieceLength)
        {
            auto next = std::next(cursor.piece);
            auto insertedIt = pieces.insert(next, insertedPiece);

            cursor.piece = insertedIt;
        }
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

            auto insertedIt = pieces.insert(cursor.piece, insertedPiece);

            auto next = std::next(insertedIt);
            pieces.insert(next, rightPiece);

            cursor.piece = insertedIt;
        }
        cursor.offset = textLength;
    }
    void deleteText(size_t start, size_t length)
    {
        size_t remaining = start;
        auto it = pieces.begin();

        moveToPosition(it, remaining);
        size_t offset = remaining;

        for (; it != pieces.end() && length > 0; ++it)
        {
            Piece currentPiece = *it;

            size_t available = currentPiece.length - offset;
            size_t take = std::min(available, length);

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
};

int main()
{

    return 0;
}
