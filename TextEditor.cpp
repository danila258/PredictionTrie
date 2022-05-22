#include "TextEditor.h"

#include <QShortcut>

TextEditor::TextEditor(QWidget* parent) : QWidget(parent), _dynamicButtonsLayout(new QHBoxLayout),
    _wordsDictionary(new PredictionTrie), _textInputField(new QTextEdit), _wordInput(new QLineEdit), _oldTextSize(0),
    _saveFlag(false)
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
    QMenu* fileMenu = new QMenu("&Menu");

    fileMenu->addAction("&Open", this, SLOT(openFile()), Qt::CTRL + Qt::Key_O);
    fileMenu->addAction("&Save", this, SLOT(saveFile()), Qt::CTRL + Qt::Key_S);
    fileMenu->addAction("&Font size", this, SLOT(menuFontSize()));

    menuBar->addMenu(fileMenu);

    return  menuBar;
}

void TextEditor::openFile()
{
    QString text;
    QString line;
    QString word;

    QString filePath = QFileDialog::getOpenFileName(this, "Open File", "", "*.*");
    QFile file(filePath);

    QVector<QString> words;

    if (!file.open(QIODevice::ReadOnly))
    {
        return;
    }

    QTextStream fileStream(&file);

    while (!fileStream.atEnd())
    {
        line = fileStream.readLine();
        text += line + "\n";

        for (int i = 0; i < line.size(); ++i) {
            if ('a' <= line[i].toLower() && line[i].toLower() <= 'z' || '0' <= line[i] && line[i] <= '9') {
                word.push_back(line[i].toLower());
            }
            else {
                words.append(word);
                qDebug() << word;
                word.clear();
            }
        }
    }

    text.remove(text.size() - 1, 1);

    for (int i = 0; i < words.size(); ++i)
    {
        if (words[i].size() > 1)
        {
            _wordsDictionary->insert(words[i].toStdString());
        }
    }

    _textInputField->setText(text);
    _oldTextSize = text.size();
    _saveFlag = false;
}

void TextEditor::saveFile()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Save File", "", "*.txt");
    QFile file(filePath);
    QString text = _textInputField->toPlainText();

    if (file.open(QIODevice::ReadWrite))
    {
        QTextStream stream(&file);
        stream << text;

        _saveFlag = true;
    }
}

void TextEditor::closeEvent(QCloseEvent *event)
{
    if (_saveFlag)
    {
        return;
    }

    QMessageBox::StandardButton userAnswer = QMessageBox::question(this, "Text Editor",
                                                                   tr("Do you want to save the file?\n"),
                                                                   QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                   QMessageBox::Yes);
    if (userAnswer == QMessageBox::Yes)
    {
        saveFile();
        event->ignore();
    }
    else if (userAnswer == QMessageBox::No)
    {
        event->accept();
    }
    else {
        event->ignore();
    }
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

    bool addToDictionary = !('a' <= text[pos - 1].toLower() && text[pos - 1].toLower() <= 'z' ||
                             '0' <= text[pos - 1] && text[pos - 1] <= '9');

    if (addToDictionary)
    {
        startPos = 2;
    }

    for (int i = pos - startPos; i > -1; --i)
    {
        if ( !('a' <= text[i].toLower() && text[i].toLower() <= 'z' || '0' <= text[i] && text[i] <= '9') )
        {
            break;
        }

        word += text[i].toLower();
    }

    std::reverse(word.begin(), word.end());

    if (addToDictionary)
    {
        if (word.size() > 1)
        {
            if ( _oldTextSize < text.size() )
            {
                _wordInput->setText(word);
                _wordsDictionary->insert(word.toStdString());
                qDebug() << "insert =" << word;
            }

            dynamicButtonsUpdate(" ");
        }
    }
    else
    {
        dynamicButtonsUpdate(word);
    }

    _oldTextSize = text.size();
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

    for (size_t i = pos - 1; ; --i)
    {
        if ( !('a' <= text[i].toLower() && text[i].toLower() <= 'z' || '0' <= text[i] && text[i] <= '9') )
        {
            break;
        }

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
    _wordsDictionary->remove( _wordInput->text().toStdString() );
    _wordInput->clear();
}

void TextEditor::shortcutDeleteWord()
{
    ((QPushButton*) ((QShortcut*) sender())->parentWidget())->animateClick();
}

void TextEditor::menuFontSize()
{
    QDialog* fontSizeWidget = new QDialog(this, Qt::WindowTitleHint | Qt::WindowSystemMenuHint);

    QVBoxLayout* mainLayout = new QVBoxLayout();
    QHBoxLayout* spinLayout = new QHBoxLayout();
    QHBoxLayout* buttonsLayout = new QHBoxLayout();

    QLabel* fontSizeLabel = new QLabel("Font size");

    QPushButton* okButton = new QPushButton("&Ok");
    QPushButton* cancelButton = new QPushButton("&Cancel");

    QSpinBox* fontSizeSpinBox = new QSpinBox();
    fontSizeSpinBox->setRange(5, 50);
    fontSizeSpinBox->setSuffix(" px");
    fontSizeSpinBox->setValue(_textInputField->font().pointSize());

    connect(fontSizeSpinBox, SIGNAL(valueChanged(int)), SLOT(checkSpinBox(int)));
    connect(okButton, SIGNAL(clicked()), SLOT(setFont()));
    connect(cancelButton, SIGNAL(clicked()), SLOT(cancelSetFont()));

    spinLayout->addWidget(fontSizeLabel);
    spinLayout->addWidget(fontSizeSpinBox);

    buttonsLayout->addWidget(okButton);
    buttonsLayout->addWidget(cancelButton);

    mainLayout->addLayout(spinLayout);
    mainLayout->addLayout(buttonsLayout);

    fontSizeWidget->setLayout(mainLayout);

    fontSizeWidget->show();
}

void TextEditor::checkSpinBox(int size)
{
    _fontSize = size;
}

void TextEditor::setFont()
{
    QString line = "font-size: " + QString::number(_fontSize) + "px";
    _textInputField->setStyleSheet(line);

    sender()->parent()->deleteLater();
}

void TextEditor::cancelSetFont()
{
    sender()->parent()->deleteLater();
}
