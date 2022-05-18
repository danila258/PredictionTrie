#include "TextEditor.h"

TextEditor::TextEditor(QWidget* parent) : QWidget(parent), _dynamicButtonsLayout(new QHBoxLayout),
    _wordsDictionary(new PredictionTrie), _textInputField(new QTextEdit)
{
    setWindowTitle("Text Editor");
    resize(500, 500);

    QVBoxLayout* mainLayout = new QVBoxLayout();
    QHBoxLayout* wordDeleteLayout = new QHBoxLayout();

    connect(_textInputField, SIGNAL(textChanged()), SLOT(userInputParser()));

    //_dynamicButtonsLayout->setSpacing(0);
    //_dynamicButtonsLayout->setContentsMargins(0, 0, 0, 0);

    //mainLayout->setSpacing(1);
    //mainLayout->setContentsMargins(5, 5, 5, 0);

    QPushButton* ghostButton = new QPushButton();
    ghostButton->setFixedSize(0, 40);

    QLineEdit* wordInput = new QLineEdit();
    QPushButton* deleteWordButton = new QPushButton("Delete");
    QPushButton* deleteLastWordButton = new QPushButton("None");

    mainLayout->addLayout(wordDeleteLayout);

    mainLayout->addWidget(_textInputField);
    wordDeleteLayout->addWidget(wordInput);
    wordDeleteLayout->addWidget(deleteWordButton);
    wordDeleteLayout->addWidget(deleteLastWordButton);
    _dynamicButtonsLayout->addWidget(ghostButton);

    mainLayout->addLayout(_dynamicButtonsLayout);

    setLayout(mainLayout);
    _textInputField->setFocus();
}

void TextEditor::userInputParser()
{
    QString text = _textInputField->toPlainText();
    QString word;
    int startPos = 1;
    bool addToDictionary = text[text.size() - 1] == ' ' || text[text.size() - 1] == '\n';

    if ( text.isEmpty() )
    {
        dynamicButtonsUpdate(" ");
        return;
    }

    if (addToDictionary)
    {
        startPos = 2;
    }

    for (int i = text.size() - startPos; i > -1; --i)
    {
        if (text[i] == ' ' || text[i] == '\n')
        {
            break;
        }

        word += text[i];
    }

    std::reverse(word.begin(), word.end());

    if (addToDictionary)
    {
        if (word.size() > 1)
        {
            qDebug() << "insert to dictionary";
            _wordsDictionary->insert(word.toStdString());
            dynamicButtonsUpdate(" ");
        }
    }
    else
    {
        qDebug() << "not insert";
        dynamicButtonsUpdate(word);
    }
}

void TextEditor::dynamicButtonsUpdate(const QString& word)
{
    qDebug() << "buttons count between del = " << _dynamicButtonsLayout->count();

    for(int i = 1; i < _dynamicButtonsLayout->count(); ++i)
    {

        QPushButton* button = static_cast<QPushButton*>(_dynamicButtonsLayout->itemAt(i)->widget());
        qDebug() << "delete button - " << i;
        button->deleteLater();
    }


    if (word == " ")
    {
        return;
    }

    qDebug() << "buttons count after del = " << _dynamicButtonsLayout->count();
    qDebug() << "spacing = " << _dynamicButtonsLayout->spacing();

    std::vector<std::string> suitableWords = _wordsDictionary->findBestMatches(word.toStdString(), 5);

    for (int i = 0; i < suitableWords.size(); ++i) {
        qDebug() << "word in suitableWords -" << QString::fromStdString(suitableWords[i]);
    }

    for (int i = 0; i < suitableWords.size(); ++i)
    {
        QPushButton* button = createDynamicButton(QString::fromStdString(suitableWords[i]));
        _dynamicButtonsLayout->addWidget(button);
    }
}

QPushButton* TextEditor::createDynamicButton(const QString& word)
{
    qDebug() << "create button - " << word;
    QPushButton* button = new QPushButton(word);
    connect(button, SIGNAL(clicked()), SLOT(autoCompleteWord()));

    return button;
}

void TextEditor::autoCompleteWord() {
    QString word = ((QPushButton*) sender())->text();
    QString text = _textInputField->toPlainText();

    for (int i = text.size() - 1; text[i] != ' ' && text[i] != '\n' && i != -1; --i) {
        if(i == 0) {
            text.clear();
            break;
        }

        text.remove(i, 1);
    }

    _textInputField->setText(text + word);
    _textInputField->setFocus();

    QTextCursor newCursor = _textInputField->textCursor();
    newCursor.movePosition(QTextCursor::End);
    _textInputField->setTextCursor(newCursor);
}
