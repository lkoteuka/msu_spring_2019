#include <cstdlib>
#include <iostream>

class Row
{
    int len;
    int* mas;

    public:
    Row(){}

    void setmas(int lens)
    {
        (*this).len = lens;
        mas = new int[len];
    }

    Row& operator=(Row& obj)
    {
        return obj;
    }

    Row& operator*=(int mn)
    {
        for(int j = 0; j < (*this).len; j++)
        {
            mas[j] *= mn;
        }
        return *this;
    }

    const int& operator[](int i ) const
    {
        if(i > len - 1)
            throw std::out_of_range("");
        return mas[i];
    }

    int& operator[](int i)
    {
        if(i > len - 1)
            throw std::out_of_range("");
        return mas[i];
    }

    ~Row()
    {
        delete[] mas;
        mas = nullptr;
    }
};

class Matrix
{
    int col;
    int row;
    Row* rows;

    public:
    Matrix(int row, int col)
    {
        (*this).col = col;
        (*this).row = row;
        rows = new Row[col];
        for(int i = 0; i < row; i++)
        {
            rows[i].setmas(col);
        }
    }

    Matrix(const Matrix &obj)
    {
        row = obj.getRows();
        col = obj.getColumns();
        rows = new Row[col];
        for(int i = 0; i < row; i++)
        {
            rows[i].setmas(col);
        }

        for(int i = 0; i < col - 1; i ++)
        {
            for(int j = 0; j < row - 1; j++)
            {
                (*this)[i][j] = obj[i][j];
            }
        }
    }
    
    int getRows() const
    {
        return (*this).row;
    }

    int getColumns() const
    {
        return (*this).col;
    }

    const Row& operator[](int i) const
    {
        if(i > row - 1)
            throw std::out_of_range("");
        return rows[i];
    }

    Row& operator[](int i)
    {
        if(i > row - 1)
            throw std::out_of_range("");

        return rows[i];
    }

    const Matrix& operator*=(int mn)
    {
        for(int j = 0; j < row; j++)
        {
            rows[j] *= mn;
        }
        return *this;
    }

    bool operator==(const Matrix& obj) const
    {
        if(this == &obj)
            return true;
        
        if((*this).row != obj.getRows() || (*this).col != obj.getColumns())
            return false;

        for(int i = 0; i < col - 1; i ++)
        {
            for(int j = 0; j < row - 1; j++)
            {
                if((*this)[i][j] != obj[i][j])
                    return false;
            }
        }

        return true;
    }

    bool operator!=(const Matrix& obj) const
    {
        return !(*this == obj);
    }

    ~Matrix()
    {
        for(int i = 0; i < row; i++)
        {
            rows[i].~Row();
        }
        delete[] rows;
        rows = nullptr;
    }
};
