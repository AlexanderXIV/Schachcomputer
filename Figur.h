#pragma once
#define ASCII

enum ChessFigure
{
  None,
  Turm,
  Springer,
  Laeufer,
  Koenig,
  Dame,
  Bauer
};

struct Figur
{
  ChessFigure type;
  bool is_white;

  explicit Figur(ChessFigure _type, bool _is_white = true) : type(_type), is_white(_is_white){};

  Figur clone() const { return Figur(type, is_white); }

  friend ostream &operator<<(ostream &os, const Figur &figur);
};

ostream &operator<<(ostream &os, const Figur &figur)
{
#ifndef ASCII
  switch (figur.type)
  {
  case None:
    os << "-";
    break;
  case Turm:
    os << ((figur.is_white) ? "♜" : "♖");
    break;
  case Springer:
    os << ((figur.is_white) ? "♞" : "♘");
    break;
  case Laeufer:
    os << ((figur.is_white) ? "♝" : "♗");
    break;
  case Koenig:
    os << ((figur.is_white) ? "♚" : "♔");
    break;
  case Dame:
    os << ((figur.is_white) ? "♛" : "♕");
    break;
  case Bauer:
    os << ((figur.is_white) ? "♟" : "♙");
    break;
  }
#else
  switch (figur.type)
  {
  case None:
    os << "--";
    break;
  case Turm:
    os << ((figur.is_white) ? "wT" : "bT");
    break;
  case Springer:
    os << ((figur.is_white) ? "wS" : "bS");
    break;
  case Laeufer:
    os << ((figur.is_white) ? "wL" : "bL");
    break;
  case Koenig:
    os << ((figur.is_white) ? "wK" : "bK");
    break;
  case Dame:
    os << ((figur.is_white) ? "wD" : "bD");
    break;
  case Bauer:
    os << ((figur.is_white) ? "wB" : "bB");
    break;
  }
#endif
  return os;
}

inline bool operator==(const Figur &a, const Figur &b) { return a.type == b.type && (None == a.type || a.is_white == b.is_white); }

inline bool operator!=(const Figur &a, const Figur &b) { return !(a == b); }