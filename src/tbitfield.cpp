// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField :: TBitField (int len)
{
	BitLen = len;
	if (BitLen > 0)
	{
		MemLen = (len + sizeof (TELEM) * 8 - 1) / (sizeof (TELEM) * 8);
		pMem = new TELEM [MemLen];
		for (int i = 0; i < MemLen; i ++)
			pMem [i] = 0;
	}
	else
		throw 1;
}

TBitField :: TBitField (const TBitField & bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM [MemLen];
	for (int i = 0; i < MemLen; i ++)
		pMem [i] = bf.pMem [i];

}

TBitField :: ~ TBitField ()
{
	delete [] pMem;
	pMem = NULL;
}

int TBitField :: GetMemIndex (const int n) const // индекс Мем для бита n
{
	return (n / (sizeof (TELEM) * 8));
}

TELEM TBitField :: GetMemMask (const int n) const // битовая маска для бита n
{
	return 1 << (n & (sizeof (TELEM) * 8 - 1));
}

// доступ к битам битового поля

int TBitField :: GetLength (void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField :: SetBit (const int n) // установить бит
{
	if ((n >= 0) && (n < BitLen))
		pMem [GetMemIndex (n)] |= GetMemMask (n);
	else
		throw 1;
}

void TBitField :: ClrBit (const int n) // очистить бит
{
	if ((n >= 0) && (n < BitLen))
		pMem [GetMemIndex (n)] &= ~GetMemMask (n);
	else
		throw 1;
}

int TBitField :: GetBit (const int n) const // получить значение бита
{
	if ((n >= 0) && (n < BitLen))
		return pMem [GetMemIndex (n)] & GetMemMask (n);
	else
		throw 1;
}

// битовые операции

TBitField & TBitField :: operator = (const TBitField & bf) //  присваивание
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	delete [] pMem;
	pMem = new TELEM [MemLen];
	for (int i = 0; i < MemLen; i ++)
		pMem [i] = bf.pMem [i];
	return * this;

}

int TBitField :: operator == (const TBitField & bf) const // сравнение
{
	if (BitLen == bf.BitLen)
    {
        int flag = 1;
        for (int i = 0; i < MemLen; i ++)
			if (pMem[i] != bf.pMem [i])
            {
                flag = 0;
                break;
            }
        return flag;
    }
    else
        return 0;
}

int TBitField :: operator != (const TBitField & bf) const // сравнение
{
  if (BitLen == bf.BitLen)
    {
        int flag = 0;
        for (int i = 0; i < MemLen; i ++)
			if (pMem[i] != bf.pMem[i])
            {
				flag = 1;
                break;
            }
			return flag;
    }
    else
		return 1;
}

TBitField TBitField :: operator | (const TBitField & bf) // операция "или"
{
	int len;
	if (BitLen > bf.BitLen)
		len = BitLen;
	else
		len = bf.BitLen;
	TBitField tmp (len);
	for (int i = 0; i < tmp.MemLen ; i ++)
		tmp.pMem [i] = bf.pMem [i];
	int mmlen;
	if (MemLen > tmp.MemLen)
		mmlen = tmp.MemLen;
	else
		mmlen = MemLen;
	for (int i = 0; i < mmlen; i ++)
		tmp.pMem [i] |= pMem [i]; // где "|=" - побитовое "ИЛИ" (OR), совмещённое с присваиванием
	return tmp;
}

TBitField TBitField :: operator & (const TBitField & bf) // операция "и"
{
	int len;
	if (BitLen > bf.BitLen)
		len = BitLen;
	else
		len = bf.BitLen;
	TBitField tmp (len);
	int mmlen;
	if (MemLen > tmp.MemLen)
		mmlen = tmp.MemLen;
	else
		mmlen = MemLen;
	for (int i = 0; i < mmlen ; i ++)
		tmp.pMem [i] = bf.pMem [i];
	for (int i = 0; i < mmlen; i ++)
		tmp.pMem [i] &= pMem [i]; // где &= - побитовое "И" (AND), совмещённое с присваиванием
	for (int i = mmlen; i < tmp.MemLen; i ++)
		tmp.pMem [i] = 0;
	return tmp;
}

TBitField TBitField :: operator ~ (void) // отрицание
{
	TBitField bf = (*this);
    for (int i = 0; i < BitLen; i ++)
    {
        if (bf.GetBit (i))
            bf.ClrBit (i);
        else
            bf.SetBit (i);
    }
    return bf;
}
 
// ввод/вывод

istream & operator >> (istream & istr, TBitField & bf) // ввод
{
	int i = 0; 
	char ch;
	do
		istr >> ch;
	while (ch != ' ');
	while (true)
	{
		istr >> ch;
		if (ch == '0')
			bf.ClrBit (i ++);
		else if (ch == '1')
			bf.SetBit (i ++);
		else 
			break;
	}
	return istr;
}

 ostream & operator << (ostream & ostr, const TBitField & bf) // вывод
{
	int len = bf.GetLength ();
	for (int i = 0; i < len; i ++)
	{
		if (bf.GetBit (i))
			ostr << '1';
		else 
			ostr << '0';
	}
	return ostr;
}
