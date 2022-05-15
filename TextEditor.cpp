#include "TextEditor.h"

TextEditor::TextEditor(QWidget* parent) : QWidget(parent){
    setWindowTitle("Text Editor");
    resize(500, 500);

    QHBoxLayout* mainLayout = new QHBoxLayout();
    QVBoxLayout* toolsLayout = new QVBoxLayout();
    QVBoxLayout* wordDeleteLayout = new QVBoxLayout();
    _dynamicButtonsLayout = new QVBoxLayout();

    _textInputField = new QTextEdit();

    QPushButton* calcButton1 = new QPushButton("1");
    QPushButton* calcButton2 = new QPushButton("2");

    mainLayout->addWidget(_textInputField);
    _dynamicButtonsLayout->addWidget(calcButton1);
    wordDeleteLayout->addWidget(calcButton2);

    toolsLayout->addLayout(_dynamicButtonsLayout);
    toolsLayout->addLayout(wordDeleteLayout);
    mainLayout->addLayout(toolsLayout);

    setLayout(mainLayout);
}
