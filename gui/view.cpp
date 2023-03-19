#include <iostream>
#include <string>
#include <windows.h>

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QFileDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QStringBuilder>
#include <QMessageBox>
#include <QDateTime>

#include "view.h"
#include "../src/utils.h"

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
            QString file_name = QFileDialog::getOpenFileName(this, "导入文件", "", "Text (*.txt)");
            if (!file_name.isEmpty()) {
                char *content = read_file(file_name.toUtf8().data());
                line->setText(file_name);
                text->setText(QString(content));
                free(content);
            }
        });
        header->addWidget(label);
        header->addWidget(line);
        text->setAcceptRichText(false);
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
        return check->isChecked() ? (char)(selector->currentIndex() + 'a') : '\0';
    }
};

class OptionView : public QWidget {
public:
    QVBoxLayout *layout = new QVBoxLayout();
    CharSelectView *head = new CharSelectView("指定首字母");
    CharSelectView *tail = new CharSelectView("指定尾字母");
    CharSelectView *reject = new CharSelectView("指定非首字母");
    QHBoxLayout *enable_loop_layout = new QHBoxLayout();
    QCheckBox *check = new QCheckBox();
    QLabel *label = new QLabel("允许隐含单词环");
    QHBoxLayout *run_layout = new QHBoxLayout();
    QComboBox *mode = new QComboBox();
    QPushButton *exec_button = new QPushButton("运行"); // 在外部注册响应器
    QLabel *time = new QLabel();

    OptionView() : QWidget() {
        enable_loop_layout->addWidget(check);
        enable_loop_layout->addWidget(label);
        enable_loop_layout->addStretch();
        mode->addItem("求所有单词链数目");
        mode->addItem("求单词数量最多的单词链");
        mode->addItem("求字母数量最多的单词链");
        connect(mode, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [&](int idx) {
            head->setEnabled(idx != 0);
            tail->setEnabled(idx != 0);
            reject->setEnabled(idx != 0);
            check->setEnabled(idx != 0);
            label->setEnabled(idx != 0);
        });
        mode->currentIndexChanged(0);
        run_layout->addWidget(mode);
        run_layout->addWidget(exec_button);
        run_layout->addStretch();
        layout->addWidget(head);
        layout->addWidget(tail);
        layout->addWidget(reject);
        layout->addLayout(enable_loop_layout);
        layout->addLayout(run_layout);
        layout->addWidget(time);
        layout->addStretch();
        this->setLayout(layout);
        this->setFixedWidth(350);
    }
};

MainView::MainView() : QWidget(), layout(new QHBoxLayout()), io_layout(new QVBoxLayout()),
                       input(new InputView()), output(new OutputView()), option(new OptionView()) {
    io_layout->addWidget(input);
    io_layout->addWidget(output);
    layout->addLayout(io_layout);
    layout->addWidget(option);
    connect(option->exec_button, &QPushButton::clicked, this, &MainView::execute);
    this->setWindowTitle("WordList");
    this->setMinimumSize(1200, 800);
    this->setLayout(layout);
    this->show();
}

char *words[MAX_WORDS_LEN + 5];
int len;
char *result[MAX_RESULT_LEN + 5];

HMODULE core;
func1 gen_chains_all;
func2 gen_chain_word, gen_chain_char;

void MainView::execute() const {
    core = LoadLibraryA("core.dll");
    gen_chains_all = (func1)GetProcAddress(core, "gen_chains_all");
    gen_chain_word = (func2)GetProcAddress(core, "gen_chain_word");
    gen_chain_char = (func2)GetProcAddress(core, "gen_chain_char");

    QString text = this->input->text->toPlainText();
    char *content = (char *)malloc(text.length() + 1);
    strcpy(content, text.toLatin1().data());
    char head = this->option->head->get_char();
    char tail = this->option->tail->get_char();
    char reject = this->option->reject->get_char();
    bool enable_loop = this->option->check->isChecked();
    int mode = this->option->mode->currentIndex();
    int r;

    parse_words(content, words, len);
    unique_words(words, len);
    result[0] = nullptr;
    try {
        qint64 start = QDateTime::currentDateTime().toMSecsSinceEpoch();
        if (mode == 0) {
            r = gen_chains_all(words, len, result, malloc);
        } else if (mode == 1) {
            r = gen_chain_word(words, len, result, head, tail, reject, enable_loop, malloc);
        } else if (mode == 2) {
            r = gen_chain_char(words, len, result, head, tail, reject, enable_loop, malloc);
        }
        qint64 end = QDateTime::currentDateTime().toMSecsSinceEpoch();

        QString res = QString::number(r) % '\n';
        for (int i = 0; i < r; ++i) {
            res = res % QString(result[i]) % '\n';
        }
        this->output->text->setText(res);
        this->option->time->setText("计算时间：" + QString::number((double)(end - start) / 1000, 'f', 3) + "s");
    } catch (const logic_error &e) {
        if (strcmp(e.what(), "Too many word chains!") == 0) {
            QMessageBox::warning((QWidget *)this, "core 错误", "单词链数目过多");
        } else {
            QMessageBox::warning((QWidget *)this, "core 错误", "存在隐含单词环");
        }
        this->output->text->clear();
        this->option->time->setText("计算错误");
    }

    free(content);
    free(result[0]);
    FreeLibrary(core);
}
