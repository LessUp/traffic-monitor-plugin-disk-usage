#pragma once
#include "PluginInterface.h"
#include <string>

// A single IPluginItem that shows the disk-usage percentage of either
// the total of all fixed drives or one specific drive letter.
class CDiskUsageItem : public IPluginItem
{
public:
    // Call UpdateData() to refresh the values shown by this item.
    // usagePercent: 0–100
    void UpdateData(float usagePercent);

    // --- IPluginItem ---
    virtual const wchar_t* GetItemName() const override;
    virtual const wchar_t* GetItemId() const override;
    virtual const wchar_t* GetItemLableText() const override;
    virtual const wchar_t* GetItemValueText() const override;
    virtual const wchar_t* GetItemValueSampleText() const override;

    virtual int IsDrawResourceUsageGraph() const override { return 1; }
    virtual float GetResourceUsageGraphValue() const override { return m_usage_value; }

private:
    float m_usage_value{ 0.0f };          // 0.0–1.0 for the graph
    mutable wchar_t m_value_text[16]{};   // e.g. "42%"
};
