#include <cstddef>
#include <list>
#include <string>
#include <string_view>

enum class Buffer
{
    Original,
    Add
};

struct Piece
{
    Buffer buffer;
    std::size_t start;
    std::size_t length;
};

struct Cursor
{
    std::list<Piece>::iterator piece;
    std::size_t offset;
};
class PieceTable
{
private:
    std::string original;
    std::string add;
    std::list<Piece> pieces;
    void moveToPiece(std::list<Piece>::iterator &it, std::size_t &remaining);

public:
    explicit PieceTable(std::string_view initialText = {});
    char charAt(std::size_t position);
    std::string getText(std::size_t start, std::size_t length);
    Cursor cursorAt(std::size_t position);
    void insertText(Cursor &cursor, std::string_view text);
    void deleteText(std::size_t start, std::size_t length);
};