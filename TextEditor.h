#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include "PredictionTrie.h"

#include <QtWidgets>


class TextEditor : public QWidget {
    Q_OBJECT

public:
    TextEditor(QWidget* parent = nullptr);

private:
    QMenuBar* topMenu();

    void dynamicButtonsUpdate(const QString& word);
    QPushButton* createDynamicButton(const QString& word, size_t number);

    QHBoxLayout* _dynamicButtonsLayout;
    QTextEdit* _textInputField;
    QLineEdit* _wordInput;

    PredictionTrie* _wordsDictionary;

    bool _saveFlag;

public:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void openFile();
    void saveFile();

    void userInputParser();

    void autoCompleteWord();
    void shortcutAutoCompleteWord();

    void deleteWord();
    void shortcutDeleteWord();

};


#endif //TEXTEDITOR_H
