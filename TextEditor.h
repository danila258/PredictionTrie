#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QtWidgets>


class TextEditor : public QWidget {
    Q_OBJECT

public:
    TextEditor(QWidget* parent = nullptr);

private:
    void userInputParser();


    QVBoxLayout* _dynamicButtonsLayout;
    QTextEdit* _textInputField;

private slots:


};


#endif //TEXTEDITOR_H
