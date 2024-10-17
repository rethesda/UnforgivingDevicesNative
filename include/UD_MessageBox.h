#pragma once

// Copied from https://github.com/SkyrimScripting/MessageBox

namespace UD
{
    // Used for testing with Cheat engine
    class MessageBoxCalibration
    {
        SINGLETONHEADER(MessageBoxCalibration)
    public:
        void Inject(RE::MessageBoxData* aData);
    private:
        uint32_t        inject = 0x00U;
        std::uint32_t   unk38; // 38
        std::int32_t    unk3C; // 3C
        std::uint32_t   unk48; // 48
        std::uint8_t    unk4C; // 4C
        std::uint8_t    unk4D; // 4D
        std::uint8_t    unk4E; // 4E
        std::uint8_t    unk4F; // 4F
    };

    class MessageboxManager
    {
    SINGLETONHEADER(MessageboxManager)
    public:

    class MessageBoxResultCallback : public RE::IMessageBoxCallback 
    {
    public:
        ~MessageBoxResultCallback() override {}
        MessageBoxResultCallback(std::function<void(uint32_t)> callback) : _callback(callback) {}
        void Run(RE::IMessageBoxCallback::Message message) override 
        {
            _callback(static_cast<uint32_t>(message));
        }
    private:
        std::function<void(uint32_t)> _callback;
    };

    void Setup();


    int32_t ShowArrayNonBlocking(const std::string& bodyText,std::vector<std::string>& buttonTexts, bool useHtml = true);
    int32_t ShowArrayNonBlockingTemplate(RE::BGSMessage* Message, std::string bodyText, std::vector<float>& values, std::vector<std::string>& buttonTexts, bool useHtml = true);
    int32_t ShowNonBlocking(const std::string& bodyText,const std::string& button1,
                            const std::string& button2 = "", const std::string& button3 = "", const std::string& button4 = "",
                            const std::string& button5 = "", const std::string& button6 = "", const std::string& button7 = "",
                            const std::string& button8 = "", const std::string& button9 = "", const std::string& button10 = "", bool useHtml = true);
    void Delete(int32_t messageBoxId);
    std::string GetResultText(int32_t messageBoxId, bool deleteResultOnAccess = true);
    int32_t GetResultIndex(int32_t messageBoxId, bool deleteResultOnAccess = true);
    bool IsMessageResultAvailable(int32_t messageBoxId);

    private:
        void ShowMessageBox(const std::string& bodyText,const std::vector<std::string>& buttonTextValues, std::function<void(uint32_t)> callback, bool useHtml);
        int32_t GetNewMessageBoxId() { return _nextMessageBoxId++; }

    private:
        std::atomic<int32_t> _nextMessageBoxId = 1;
        std::unordered_map<int32_t, int32_t> _messageBoxResults;
        std::unordered_map<int32_t, std::vector<std::string>> _messageBoxButtons;
        std::unordered_map<int32_t, std::vector<int32_t>> _messageBoxButtonsResParse; // needed to take in account buttons hidden by conditions
    };



    // **************************************************
    //                 PAPYRUS FUNCTION
    // **************************************************

    inline int32_t ShowArrayNonBlocking(PAPYRUSFUNCHANDLE, std::string bodyText, std::vector<std::string> buttonTexts, bool useHtml) 
    {
        return MessageboxManager::GetSingleton()->ShowArrayNonBlocking(bodyText,buttonTexts,useHtml);
    }
    inline int32_t ShowArrayNonBlockingTemplate(PAPYRUSFUNCHANDLE, RE::BGSMessage* Message, std::string bodyText, std::vector<float> values, std::vector<std::string> buttonTexts, bool useHtml) 
    {
        return MessageboxManager::GetSingleton()->ShowArrayNonBlockingTemplate(Message, bodyText, values,buttonTexts,useHtml);
    }

    inline int32_t ShowNonBlocking(PAPYRUSFUNCHANDLE, std::string bodyText, std::string button1,
                                    std::string button2, std::string button3, std::string button4,
                                    std::string button5, std::string button6, std::string button7,
                                    std::string button8, std::string button9, std::string button10, bool useHtml) 
    {
        return MessageboxManager::GetSingleton()->ShowNonBlocking(bodyText,button1,button2,button3,button4,button5,button6,button7,button8,button9,button10,useHtml);
    }

    inline void Delete(PAPYRUSFUNCHANDLE, int32_t messageBoxId) 
    {
        MessageboxManager::GetSingleton()->Delete(messageBoxId);
    }

    inline std::string GetResultText(PAPYRUSFUNCHANDLE, int32_t messageBoxId, bool deleteResultOnAccess) 
    {
        return MessageboxManager::GetSingleton()->GetResultText(messageBoxId,deleteResultOnAccess);
    }

    inline int32_t GetResultIndex(PAPYRUSFUNCHANDLE, int32_t messageBoxId, bool deleteResultOnAccess)
    {
        return MessageboxManager::GetSingleton()->GetResultIndex(messageBoxId,deleteResultOnAccess);
    }

    inline bool IsMessageResultAvailable(PAPYRUSFUNCHANDLE, int32_t messageBoxId)
    {
        return MessageboxManager::GetSingleton()->IsMessageResultAvailable(messageBoxId);
    }
}