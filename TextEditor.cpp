#include "TextEditor.h"

TextEditor::TextEditor(QWidget* parent) : QWidget(parent), _dynamicButtonsLayout(new QHBoxLayout),
    _wordsDictionary(new PredictionTrie), _textInputField(new QTextEdit)
{
    setWindowTitle("Text Editor");
    resize(500, 500);

    QVBoxLayout* mainLayout = new QVBoxLayout();
    QVBoxLayout* wordDeleteLayout = new QVBoxLayout();

    connect(_textInputField, SIGNAL(textChanged()), SLOT(userInputParser()));

    //_dynamicButtonsLayout->setSpacing(0);
    //_dynamicButtonsLayout->setContentsMargins(0, 0, 0, 0);

    //mainLayout->setSpacing(0);
    //mainLayout->setContentsMargins(0, 0, 0, 0);

    QPushButton* calcButton2 = new QPushButton("2");

    mainLayout->addWidget(_textInputField);
    wordDeleteLayout->addWidget(calcButton2);

    //toolsLayout->addLayout(_dynamicButtonsLayout);
    //toolsLayout->addLayout(wordDeleteLayout);
    mainLayout->addLayout(_dynamicButtonsLayout);

    setLayout(mainLayout);
}

void TextEditor::userInputParser()
{
    QString text = _textInputField->toPlainText();
    QString word;
    int startPos = 1;

    if ( text.isEmpty() )
    {
        dynamicButtonsUpdate(" ");
        return;
    }

    if (text[text.size() - 1] == ' ')
    {
        startPos = 2;
    }

    for (int i = text.size() - startPos; i > -1; --i)
    {
        if (text[i] == ' ')
        {
            break;
        }

        word += text[i];
    }

    std::reverse(word.begin(), word.end());

    if (text[text.size() - 1] == ' ')
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

    for(int i = 0; i < _dynamicButtonsLayout->count(); ++i)
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
        _dynamicButtonsLayout->addWidget(button, 10, Qt::AlignTop);
    }
}

QPushButton* TextEditor::createDynamicButton(const QString& word)
{
    qDebug() << "create button - " << word;
    QPushButton* button = new QPushButton(word);
    connect(button, SIGNAL(clicked()), SLOT(dynamicButtonClicked()));

    return button;
}

void TextEditor::dynamicButtonClicked() {
    QString word = ((QPushButton*) sender())->text();
    QString text = _textInputField->toPlainText() + word;

    _textInputField->setText(text);
}
