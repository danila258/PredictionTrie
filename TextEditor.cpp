#include "TextEditor.h"

#include <QShortcut>

TextEditor::TextEditor(QWidget* parent) : QWidget(parent), _dynamicButtonsLayout(new QHBoxLayout),
    _wordsDictionary(new PredictionTrie), _textInputField(new QTextEdit), _wordInput(new QLineEdit)
{
    setWindowTitle("Text Editor");
    resize(500, 500);

    QVBoxLayout* mainLayout = new QVBoxLayout();
    QHBoxLayout* wordDeleteLayout = new QHBoxLayout();

    QPushButton* ghostButton = new QPushButton();
    ghostButton->setFixedSize(0, 40);

    QPushButton* deleteWordButton = new QPushButton("Delete");
    QShortcut* shortcutDelete = new QShortcut(QKeySequence("Ctrl+D"), deleteWordButton);

    connect(deleteWordButton, SIGNAL(clicked()), SLOT(deleteWord()));
    connect(shortcutDelete, SIGNAL(activated()), SLOT(shortcutDeleteWord()));
    connect(_textInputField, SIGNAL(cursorPositionChanged()), SLOT(userInputParser()));

    wordDeleteLayout->addWidget(_wordInput);
    wordDeleteLayout->addWidget(deleteWordButton);

    _dynamicButtonsLayout->addWidget(ghostButton);
    _dynamicButtonsLayout->setSpacing(0);

    mainLayout->setMenuBar(topMenu());
    mainLayout->addLayout(wordDeleteLayout);
    mainLayout->addWidget(_textInputField);
    mainLayout->addLayout(_dynamicButtonsLayout);

    setLayout(mainLayout);
    _textInputField->setFocus();
}

QMenuBar* TextEditor::topMenu()
{
    QMenuBar* menuBar = new QMenuBar(this);
    QMenu* fileMenu = new QMenu("&File");

    fileMenu->addAction("&Open", this, SLOT(openFile()), Qt::CTRL + Qt::Key_O);
    fileMenu->addAction("&Save", this, SLOT(saveFile()), Qt::CTRL + Qt::Key_S);

    menuBar->addMenu(fileMenu);

    return  menuBar;
}

void TextEditor::openFile() {

}

void TextEditor::saveFile() {

}

void TextEditor::userInputParser()
{
    QString text = _textInputField->toPlainText();
    QString word;
    int startPos = 1;

    QTextCursor cursor = _textInputField->textCursor();
    size_t pos = cursor.position();

    if ( text.isEmpty() || pos == 0)
    {
        dynamicButtonsUpdate(" ");
        return;
    }

    bool addToDictionary = text[pos - 1] == ' ' || text[pos - 1] == '\n';

    if (addToDictionary)
    {
        startPos = 2;
    }

    for (int i = pos - startPos; i > -1; --i)
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
            _wordInput->setText(word);
            _wordsDictionary->insert(word.toStdString());
            dynamicButtonsUpdate(" ");
        }
    }
    else
    {
        dynamicButtonsUpdate(word);
    }
}

void TextEditor::dynamicButtonsUpdate(const QString& word)
{
    for(int i = 1; i < _dynamicButtonsLayout->count(); ++i)
    {
        QPushButton* button = static_cast<QPushButton*>(_dynamicButtonsLayout->itemAt(i)->widget());
        button->deleteLater();
    }

    std::vector<std::string> suitableWords = _wordsDictionary->findBestMatches(word.toStdString(), 5);

    for (int i = 0; i < suitableWords.size(); ++i)
    {
        QPushButton* button = createDynamicButton(QString::fromStdString(suitableWords[i]), i + 1);
        _dynamicButtonsLayout->addWidget(button);
    }
}

QPushButton* TextEditor::createDynamicButton(const QString& word, size_t number)
{
    QPushButton* button = new QPushButton(word);

    QString shortcutStr = "Ctrl+" + QString::number(number);
    QShortcut* shortcut = new QShortcut(QKeySequence(shortcutStr), button);

    connect(button, SIGNAL(clicked()), SLOT(autoCompleteWord()));
    connect(shortcut, SIGNAL(activated()), SLOT(shortcutAutoCompleteWord()));

    return button;
}

void TextEditor::autoCompleteWord()
{
    QString word = ((QPushButton*) sender())->text();
    QString text = _textInputField->toPlainText();

    QTextCursor cursor = _textInputField->textCursor();
    size_t pos = cursor.position();

    size_t delCount = 0;

    for (size_t i = pos - 1; text[i] != ' ' && text[i] != '\n'; --i)
    {
        delCount += 1;

        if (i == 0)
        {
            break;
        }
    }

    size_t newPos = pos - delCount + word.size();

    _textInputField->setText(text.insert(pos, word.remove(0, delCount)));
    _textInputField->setFocus();

    cursor.setPosition(newPos);
    _textInputField->setTextCursor(cursor);
}

void TextEditor::shortcutAutoCompleteWord()
{
    ((QPushButton*) ((QShortcut*) sender())->parentWidget())->animateClick();
}

void TextEditor::deleteWord()
{
    QString word = _wordInput->text();

    if (!_wordsDictionary->isPresented( word.toStdString() ))
    {
        qDebug() << "no word = " << word;
        _wordInput->setText("error");
    }
    else
    {
        qDebug() << "remove = " << word;
        _wordsDictionary->remove( word.toStdString() );
        _wordInput->clear();
    }
}

void TextEditor::shortcutDeleteWord()
{
    ((QPushButton*) ((QShortcut*) sender())->parentWidget())->animateClick();
}