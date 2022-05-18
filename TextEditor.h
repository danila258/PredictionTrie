#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include "PredictionTrie.h"

#include <QtWidgets>


class TextEditor : public QWidget {
    Q_OBJECT

public:
    TextEditor(QWidget* parent = nullptr);

private:
    void dynamicButtonsUpdate(const QString& word);
    QPushButton* createDynamicButton(const QString& word, size_t number);

    QHBoxLayout* _dynamicButtonsLayout;
    QTextEdit* _textInputField;
    QLineEdit* _wordInput;

    PredictionTrie* _wordsDictionary;

private slots:
    void userInputParser();
    void autoCompleteWord();
    void shortcutButton();
};


#endif //TEXTEDITOR_H
