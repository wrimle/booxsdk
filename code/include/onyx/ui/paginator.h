
#ifndef ONYX_PAGINATOR_H_
#define ONYX_PAGINATOR_H_

class Paginator
{
public:
    Paginator();
    ~Paginator();

public:
    void reset(int first, int items_per_page, int total);

    bool prev();
    bool next();
    bool jump(int new_page);

    bool isPrevEnable();
    bool isNextEnable();

    void resize(int new_items_per_page);

    int currentPage();
    int pages();

    int first_visible() { return first_visible_; }
    int last_visible();

    int offsetInCurrentPage();

    int size() { return size_; }
    int items_per_page() { return items_per_page_; }

    int rows() { return rows_; }
    int cols() { return cols_; }
    void setGrid(const int r, const int c) { rows_ = r; cols_ = c; }

    int user_data() { return user_data_; }
    void setUserData(int d) { user_data_ = d; }

private:
    int first_visible_;
    int items_per_page_;
    int size_;
    int rows_;
    int cols_;
    int user_data_;
};

#endif      // ONYX_PAGINATOR_H_
