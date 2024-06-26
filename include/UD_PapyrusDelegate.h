#pragma once

#include <UD_Spinlock.h>

namespace UD
{
    #define UDBASESCRIPT "ud_customdevice_renderscript"

    using InternalVM = RE::BSScript::Internal::VirtualMachine;
    using Script = RE::BSTTuple<const RE::VMHandle, RE::BSTSmallSharedArray<RE::BSScript::Internal::AttachedScript>>;

    struct Device
    {
        RE::BSTSmartPointer<RE::BSScript::Object> object;
        RE::TESObjectARMO* id;
        RE::TESObjectARMO* rd;
        uint32_t           wearer;
    };

    typedef RE::TESObjectARMO*(* GetDeviceRender)(RE::TESObjectARMO*);

    class PapyrusDelegate
    {
    SINGLETONHEADER(PapyrusDelegate)
    public:
        enum Result : uint8_t
        {
            rSuccess        = 0,
            rArgError       = 1,
            rDeviceError    = 2,
            rNotFound       = 3
        };

        enum MinigameThreads : uint8_t
        {
            tStarter = 0x01,
            tCrit    = 0x02,
            tParalel = 0x04,
            tAV      = 0x08
        };

        struct FilterDeviceResult
        {
            bool Result;
            RE::TESObjectARMO* id;
            RE::TESObjectARMO* rd;
        };

        static RE::VMHandle ToVMHandle(const int a_1, const int a_2);

        void Setup();
        void Reload();

        int SendRegisterDeviceScriptEvent(RE::Actor* a_actor,std::vector<RE::TESObjectARMO*>& a_devices);
        Result SendMinigameThreadEvents(RE::Actor* a_actor,RE::TESObjectARMO* a_device,RE::VMHandle a_handle,MinigameThreads a_threads);
        Result SendRemoveRenderDeviceEvent(RE::Actor* a_actor,RE::TESObjectARMO* a_device);
        Result SetBitMapData(RE::VMHandle a_handle,RE::TESObjectARMO* a_device,std::string a_name,int a_val,uint8_t a_size,uint8_t a_off);
        void UpdateVMHandles() const;
        Device GetDeviceScript(int a_handle1,int a_handle2,RE::TESObjectARMO* a_device);
        Device GetCachedDevice(RE::VMHandle,RE::Actor* a_actor, RE::TESObjectARMO* a_device);

        void Lock();
        void Unlock();
    private:
        RE::VMHandle ValidateVMHandle(RE::VMHandle a_handle,RE::TESObjectARMO* a_device);
        void ValidateCache() const;
        RE::BSScript::ObjectTypeInfo* IsUnforgivingDevice(RE::BSTSmallSharedArray<RE::BSScript::Internal::AttachedScript>& a_scripts) const;
        FilterDeviceResult CheckRegisterDevice(RE::VMHandle a_handle,RE::BSScript::ObjectTypeInfo* a_type,RE::Actor* a_actor, std::vector<RE::TESObjectARMO*>& a_devices);
        FilterDeviceResult ProcessDevice(RE::VMHandle a_handle,RE::VMHandle a_handle2,RE::BSScript::ObjectTypeInfo* a_type,RE::Actor* a_actor, std::vector<RE::TESObjectARMO*>& a_devices,std::function<void(RE::BSTSmartPointer<RE::BSScript::Object>,RE::TESObjectARMO*,RE::TESObjectARMO*)> a_fun);
        FilterDeviceResult ProcessDevice2(RE::VMHandle a_handle,RE::VMHandle a_handle2,RE::BSScript::ObjectTypeInfo* a_type,RE::TESObjectARMO* a_device,std::function<bool(RE::BSTSmartPointer<RE::BSScript::Object>,RE::TESObjectARMO*,RE::TESObjectARMO*)> a_fun);
        GetDeviceRender DDNGGetDeviceRender;
        void ResetCache();
    private:
        bool _installed = false;
        RE::BGSKeyword* _udrdkw;
        template<class T> T* GetScriptVariable(RE::BSTSmartPointer<RE::BSScript::Object> a_scriptobject, RE::BSFixedString a_variable,RE::FormType a_type) const;
        template<class T> T* GetScriptProperty(RE::BSTSmartPointer<RE::BSScript::Object> a_scriptobject, RE::BSFixedString a_property,RE::FormType a_type) const;
        
        mutable uint64_t _RemovedCounter = 0x0; //removed devices counter
        mutable std::unordered_map<RE::VMHandle,Device> _cache;
        mutable Utils::Spinlock _SaveLock;
    };

    inline int SendRegisterDeviceScriptEvent(PAPYRUSFUNCHANDLE,RE::Actor* a_actor,std::vector<RE::TESObjectARMO*> a_devices)
    {
        PapyrusDelegate::GetSingleton()->Lock();
        auto loc_res = PapyrusDelegate::GetSingleton()->SendRegisterDeviceScriptEvent(a_actor,a_devices);
        PapyrusDelegate::GetSingleton()->Unlock();
        return loc_res;
    }

    inline int SendMinigameThreadEvents(PAPYRUSFUNCHANDLE,RE::Actor* a_actor,RE::TESObjectARMO* a_device,int a_handle1,int a_handle2, int a_mode)
    {
        PapyrusDelegate::GetSingleton()->Lock();
        auto loc_res = static_cast<int>(PapyrusDelegate::GetSingleton()->SendMinigameThreadEvents(a_actor,a_device,PapyrusDelegate::ToVMHandle(a_handle1,a_handle2),(PapyrusDelegate::MinigameThreads)a_mode));
        PapyrusDelegate::GetSingleton()->Unlock();
        return loc_res;
    }

    inline int SendRemoveRenderDeviceEvent(PAPYRUSFUNCHANDLE,RE::Actor* a_actor,RE::TESObjectARMO* a_device)
    {
        PapyrusDelegate::GetSingleton()->Lock();
        auto loc_res = static_cast<int>(PapyrusDelegate::GetSingleton()->SendRemoveRenderDeviceEvent(a_actor,a_device));
        PapyrusDelegate::GetSingleton()->Unlock();
        return loc_res;
    }

    inline int SetBitMapData(PAPYRUSFUNCHANDLE,int a_handle1,int a_handle2,RE::TESObjectARMO* a_device,std::string a_name,int a_val,int a_size,int a_off)
    {
        PapyrusDelegate::GetSingleton()->Lock();
        const auto loc_handle = PapyrusDelegate::ToVMHandle(a_handle1,a_handle2);
        auto loc_res = static_cast<int>(PapyrusDelegate::GetSingleton()->SetBitMapData(loc_handle,a_device,a_name,a_val,a_size,a_off));
        PapyrusDelegate::GetSingleton()->Unlock();
        return loc_res;
    }

    inline void UpdateVMHandles(PAPYRUSFUNCHANDLE)
    {
        PapyrusDelegate::GetSingleton()->Lock();
        PapyrusDelegate::GetSingleton()->UpdateVMHandles();
        PapyrusDelegate::GetSingleton()->Unlock();
    }
}