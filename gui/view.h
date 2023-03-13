#ifndef WORDLIST_VIEW_H
#define WORDLIST_VIEW_H

#include <QWidget>
#include <QMainWindow>
#include <QLayout>

class InputView;

class OutputView;

class OptionView;

class MainView : public QWidget {
public:
    QHBoxLayout *layout;
    QVBoxLayout *io_layout;
    InputView *input;
    OutputView *output;
    OptionView *option;

    MainView();

    void execute() const;
};

typedef int (*func1)(char *[], int, char *[], void *(size_t));

typedef int (*func2)(char *[], int, char *[], char, char, char, bool, void *(size_t));

#endif //WORDLIST_VIEW_H
