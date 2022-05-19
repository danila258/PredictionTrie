#include "PredictionTrie.h"

#include <algorithm>
#include <stdexcept>
#include <iostream>

PredictionTrie::PredictionTrie()
{
    _root = new PredictionTrie::PredictionTrieNode;
    _root->count = 0u;
    _root->type = PredictionTrie::PredictionTrieNode::Type::Root;
}

PredictionTrie::~PredictionTrie()
{
    for (auto& i : _nodeVector) {
        delete i;
    }
}

void PredictionTrie::insert(const std::string& word)
{
    qDebug() << "insert ->" << QString::fromStdString(word);

    auto* current = _root;

    for (auto letter : word)
    {
        auto foundIt = current->children.find(letter);
        _nodeVector.push_back(current);

        if (foundIt == current->children.end())
        {
            auto [it, _] = current->children.emplace(letter, new PredictionTrieNode);
            foundIt = it;
        }

        current = foundIt->second;
    }

    current->type = PredictionTrie::PredictionTrieNode::Type::Leaf;
    current->count += 1u;
}

void PredictionTrie::remove(const std::string& word)
{
    std::string wordCopy = word;

    if ( !isPresented(word) ) {
        return;
    }

    for (int i = word.size() - 1; i > -1; --i)
    {
        auto* current = find(wordCopy.erase(i + 1, word.size() - 1 - i));

        if (!current->children.empty() && i == word.size() - 1)
        {
            current->count = 0;
            continue;
        }

        if (current->children.size() > 1)
        {

            current->children.erase(word[i + 1]);

            if (!current->children.empty())
            {
                current->type = PredictionTrie::PredictionTrieNode::Type::Leaf;
                break;
            }
        }
    }
}

PredictionTrie::PredictionTrieNode* PredictionTrie::find(const std::string& word) const
{
    auto* current = _root;
    for (auto letter : word)
    {
        auto foundIt = current->children.find(letter);

        if (foundIt == current->children.end())
        {
            return nullptr;
        }

        current = foundIt->second;
    }

    return current;
}

bool PredictionTrie::isPresented(const std::string& word) const
{
    auto* found = find(word);

    return found && found->type == PredictionTrie::PredictionTrieNode::Type::Leaf;
}

std::vector<PredictionTrie::MatchedPair> PredictionTrie::allWordsStartedWith(const std::string& wordBegin) const
{
    auto* found = find(wordBegin);
    if (!found)
    {
        return {};
    }

    std::vector<MatchedPair> result;
    collectAllWordsStartedWith(wordBegin, found->children, result);

    return result;
}

void PredictionTrie::collectAllWordsStartedWith(
        const std::string& wordPart, const std::unordered_map<char, PredictionTrieNode*>& letterLayer,
        std::vector<MatchedPair>& result) const
{
    for (auto&& [letter, node] : letterLayer)
    {
        std::string word = wordPart + letter;
        if (node->type == PredictionTrie::PredictionTrieNode::Type::Leaf)
        {
            result.push_back(std::make_tuple(word, node->count));
        }

        collectAllWordsStartedWith(word, node->children, result);
    }
}

std::string PredictionTrie::findBestMatch(const std::string& wordBegin) const
{
    return findBestMatches(wordBegin, 1).front();
}

std::vector<std::string> PredictionTrie::findBestMatches(const std::string& wordBegin, unsigned count) const
{
    auto allWords = allWordsStartedWith(wordBegin);

    count = allWords.size() < count ? allWords.size() : count;

    qDebug() << "найденных слов" << count;

    std::partial_sort(
            std::begin(allWords),
            allWords.begin() + count,
            std::end(allWords),
            [](const auto& first, const auto& second) {
                return std::get<1>(first) > std::get<1>(second);
            }
    );

    std::vector<std::string> result;

    for (int i = 0; i < count; ++i) {
        result.push_back(std::get<0>(allWords[i]));
        qDebug() << QString::fromStdString(result[i]);
    }

    return result;
}

