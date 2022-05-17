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
    QPushButton* createDynamicButton(const QString& word);

    QHBoxLayout* _dynamicButtonsLayout;
    QTextEdit* _textInputField;

    PredictionTrie* _wordsDictionary;

private slots:
    void userInputParser();
    void dynamicButtonClicked();

};


#endif //TEXTEDITOR_H
