#include <iostream>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QFileDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>

#include "view.h"
#include "utils.h"

using namespace std;

class InputView : public QWidget {
public:
    QVBoxLayout *layout = new QVBoxLayout();
    QHBoxLayout *header = new QHBoxLayout();
    QLabel *label = new QLabel("输入：");
    QLineEdit *line = new QLineEdit();
    QPushButton *button = new QPushButton("导入文件");
    QTextEdit *text = new QTextEdit();

    InputView() : QWidget() {
        line->setReadOnly(true);
        connect(button, &QPushButton::clicked, [&]() {
            char *context;
            int size;
            QString file_name = QFileDialog::getOpenFileName(this, "导入文件", "", "Text (*.txt)");
            line->setText(file_name);
            read_file(file_name.toUtf8().data(), context, size);
            text->setText(QString(context));
            free(context);
        });
        header->addWidget(label);
        header->addWidget(line);
        header->addWidget(button);
        layout->addLayout(header);
        layout->addWidget(text);
        this->setLayout(layout);
    }
};

class OutputView : public QWidget {
public:
    QVBoxLayout *layout = new QVBoxLayout();
    QHBoxLayout *header = new QHBoxLayout();
    QLabel *label = new QLabel("输出：");
    QPushButton *button = new QPushButton("导出结果");
    QTextEdit *text = new QTextEdit();

    OutputView() : QWidget() {
        connect(button, &QPushButton::clicked, [&]() {
            QString file_name = QFileDialog::getSaveFileName(this, "导出结果", "solution", "Text (*.txt)");
            FILE *file = fopen(file_name.toUtf8().data(), "wb");
            QString result = text->toPlainText();
            fwrite(result.toUtf8().data(), 1, result.length(), file);
            fclose(file);
        });
        header->addWidget(label);
        header->addStretch();
        header->addWidget(button);
        text->setReadOnly(true);
        layout->addLayout(header);
        layout->addWidget(text);
        this->setLayout(layout);
    }
};

class CharSelectView : public QWidget {
public:
    QHBoxLayout *layout = new QHBoxLayout();
    QCheckBox *check = new QCheckBox();
    QLabel *label = new QLabel();
    QComboBox *selector = new QComboBox();

    explicit CharSelectView(const QString &s) : QWidget() {
        connect(check, &QCheckBox::stateChanged, [&]() {
            selector->setDisabled(!check->isChecked());
        });
        label->setText(s);
        for (int i = 0; i < 26; ++i) {
            selector->addItem(QString('a' + i));
        }
        selector->setDisabled(true);
        layout->addWidget(check);
        layout->addWidget(label);
        layout->addWidget(selector);
        layout->addStretch();
        layout->setContentsMargins(0, 0, 0, 0);
        this->setLayout(layout);
    }

    char get_char() const {
        return (char)(selector->currentIndex() + 'a');
    }
};

class OptionView : public QWidget {
public:
    QVBoxLayout *layout = new QVBoxLayout();
    CharSelectView *begin_with = new CharSelectView("指定首字母");
    CharSelectView *end_with = new CharSelectView("指定尾字母");
    CharSelectView *not_begin_with = new CharSelectView("指定非首字母");
    QHBoxLayout *allow_cycle_layout = new QHBoxLayout();
    QCheckBox *check = new QCheckBox();
    QLabel *label = new QLabel("允许隐含单词环");
    QHBoxLayout *run_layout = new QHBoxLayout();
    QComboBox *mode = new QComboBox();
    QPushButton *button = new QPushButton("运行"); // 在外部绑定事件

    OptionView() : QWidget() {
        allow_cycle_layout->addWidget(check);
        allow_cycle_layout->addWidget(label);
        allow_cycle_layout->addStretch();
        mode->addItem("求所有单词链数目");
        mode->addItem("求单词数量最多的单词链");
        mode->addItem("求字母数量最多的单词链");
        run_layout->addWidget(mode);
        run_layout->addWidget(button);
        run_layout->addStretch();
        layout->addWidget(begin_with);
        layout->addWidget(end_with);
        layout->addWidget(not_begin_with);
        layout->addLayout(allow_cycle_layout);
        layout->addLayout(run_layout);
        layout->addStretch();
        this->setLayout(layout);
        this->setFixedWidth(300);
    }
};

MainView::MainView() : QWidget(), layout(new QHBoxLayout()), io_layout(new QVBoxLayout()),
                       input(new InputView()), output(new OutputView()), option(new OptionView()) {
    io_layout->addWidget(input);
    io_layout->addWidget(output);
    layout->addLayout(io_layout);
    layout->addWidget(option);
    connect(option->button, &QPushButton::clicked, [&]() {
        // TODO
    });
    this->setWindowTitle("WordList");
    this->setMinimumSize(1200, 800);
    this->setLayout(layout);
    this->setAttribute(Qt::WA_DeleteOnClose, true);
    this->show();
}
