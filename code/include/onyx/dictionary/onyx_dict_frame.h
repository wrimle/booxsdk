#ifndef ONYX_DICT_FRAME_H_
#define ONYX_DICT_FRAME_H_

#include "onyx/base/base.h"
#include "onyx/ui/catalog_view.h"
#include "onyx/ui/ui.h"
#include "onyx/sys/sys.h"
#include "onyx/tts/tts.h"
#include "dictionary_manager.h"

using namespace ui;
using namespace base;

class OnyxDictFrame: public OnyxDialog
{
    Q_OBJECT

public:
    OnyxDictFrame(QWidget *parent, DictionaryManager & dict, tts::TTS *tts = 0);
    ~OnyxDictFrame();

    bool lookup(const QString &word);
    void setDefaultFocus();

Q_SIGNALS:
    void rotateScreen();
    void keyReleaseSignal(int);
    void closeClicked();

protected:
    void resizeEvent(QResizeEvent *e);

private:
    void createLineEdit();
    void createSubMenu();
    void createDictionaryMenu();
    void createTtsButtonView();
    void createLayout();

    void onItemActivated(CatalogView *catalog, ContentView *item,
                                       int user_data);

private:
    QVBoxLayout big_layout_;
    QHBoxLayout line_edit_layout_;
    QHBoxLayout dict_menu_layout_;

    CatalogView line_edit_;
    CatalogView sub_menu_;
    OnyxTextBrowser explanation_;       ///< The lookup result.
    OnyxTreeView words_list_widget_;
    CatalogView dictionary_menu_;
    CatalogView tts_button_view_;

    OnyxKeyboard keyboard_;
    StatusBar status_bar_;              ///< Status Bar

    DictionaryManager &dict_mgr_;
    tts::TTS *tts_engine_;

    QButtonGroup button_group_;
    QTextDocument doc_;
    QString word_;                      ///< Word currently queried.
    QStringList similar_words_;
    int similar_words_offset_;
    QTimer timer_;                      ///< Timer to update the screen.

    QStandardItemModel similar_words_model_;
    QStandardItemModel dict_list_model_;

    int internal_state_;
    SystemActions system_actions_;      ///< System Actions

private:
    NO_COPY_AND_ASSIGN(OnyxDictFrame);
};

#endif
