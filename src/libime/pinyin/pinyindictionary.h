/*
 * Copyright (C) 2017~2017 by CSSlayer
 * wengxt@gmail.com
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of the
 * License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; see the file COPYING. If not,
 * see <http://www.gnu.org/licenses/>.
 */
#ifndef _FCITX_LIBIME_PINYIN_PINYINDICTIONARY_H_
#define _FCITX_LIBIME_PINYIN_PINYINDICTIONARY_H_

#include "libimepinyin_export.h"
#include <fcitx-utils/connectableobject.h>
#include <fcitx-utils/macros.h>
#include <libime/core/datrie.h>
#include <libime/core/dictionary.h>
#include <libime/pinyin/pinyinencoder.h>
#include <memory>

namespace libime {

enum class PinyinDictFormat { Text, Binary };

class PinyinDictionaryPrivate;

typedef std::function<bool(boost::string_view encodedPinyin,
                           boost::string_view hanzi, float cost)>
    PinyinMatchCallback;
class PinyinDictionary;

using PinyinTrie = DATrie<float>;

class LIBIMEPINYIN_EXPORT PinyinDictionary : public Dictionary,
                                             public fcitx::ConnectableObject {
public:
    static const size_t SystemDict = 0;
    static const size_t UserDict = 1;
    explicit PinyinDictionary();
    ~PinyinDictionary();

    // Load dicitonary for a specific dict.
    void load(size_t idx, std::istream &in, PinyinDictFormat format);
    void load(size_t idx, const char *filename, PinyinDictFormat format);

    // Append a dictionary at the end.
    void addEmptyDict();

    // Match the word by encoded pinyin.
    void matchWords(const char *data, size_t size,
                    PinyinMatchCallback callback) const;

    void save(size_t idx, const char *filename, PinyinDictFormat format);
    void save(size_t idx, std::ostream &out, PinyinDictFormat format);

    // Remove a dictionary by index. The idx after it will be invalided. But the
    // pointer to PinyinTrie will not.
    void remove(size_t idx);
    const PinyinTrie *trie(size_t idx) const;

    // Total number to dictionary.
    size_t dictSize() const;

    void addWord(size_t idx, boost::string_view fullPinyin,
                 boost::string_view hanzi, float cost = 0.0f);

    FCITX_DECLARE_SIGNAL(PinyinDictionary, dictionaryChanged, void(size_t));

protected:
    void
    matchPrefixImpl(const SegmentGraph &graph,
                    const GraphMatchCallback &callback,
                    const std::unordered_set<const SegmentGraphNode *> &ignore,
                    void *helper) const override;

private:
    void loadText(size_t idx, std::istream &in);
    void loadBinary(size_t idx, std::istream &in);
    void saveText(size_t idx, std::ostream &out);

    std::unique_ptr<PinyinDictionaryPrivate> d_ptr;
    FCITX_DECLARE_PRIVATE(PinyinDictionary);
};
}

#endif // _FCITX_LIBIME_PINYIN_PINYINDICTIONARY_H_