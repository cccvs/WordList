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
};

#endif //WORDLIST_VIEW_H
