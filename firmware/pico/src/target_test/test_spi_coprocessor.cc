#include "hardware_unit_tests.hh"
#include "spi_coprocessor.hh"

UTEST(SpiCoprocessor, WriteReadScratchNoAck) {
    uint32_t scratch_out = 0xDEADBEEF;
    ASSERT_TRUE(esp32.Write(ObjectDictionary::Address::kAddrScratch, scratch_out));
    uint32_t scratch_in = 0x0;
    ASSERT_TRUE(esp32.Read(ObjectDictionary::Address::kAddrScratch, scratch_in));
    EXPECT_EQ(scratch_out, scratch_in);
}

UTEST(SpiCoprocessor, WriteReadScratchWithAck) {
    uint32_t scratch_out = 0xDEADBEEF;
    // Write requires an ack.
    ASSERT_TRUE(esp32.Write(ObjectDictionary::Address::kAddrScratch, scratch_out, true));
    uint32_t scratch_in = 0x0;
    ASSERT_TRUE(esp32.Read(ObjectDictionary::Address::kAddrScratch, scratch_in));
    EXPECT_EQ(scratch_out, scratch_in);
}

UTEST(SPICoprocessor, ReadWriteReadRewriteRereadBigNoAck) {
    SettingsManager::Settings settings_in_original;
    memset(&settings_in_original, 0x0, sizeof(settings_in_original));
    ASSERT_TRUE(esp32.Read(ObjectDictionary::Address::kAddrSettingsStruct, settings_in_original));
    SettingsManager::Settings settings_out;
    for (uint16_t i = 0; i < sizeof(settings_out); i++) {
        ((uint8_t *)&settings_out)[i] = i % UINT8_MAX;
    }
    ASSERT_TRUE(esp32.Write(ObjectDictionary::Address::kAddrSettingsStruct, settings_out));
    SettingsManager::Settings settings_in_modified;
    ASSERT_TRUE(esp32.Read(ObjectDictionary::Address::kAddrSettingsStruct, settings_in_modified));
    for (uint16_t i = 0; i < sizeof(settings_out); i++) {
        EXPECT_EQ(i % UINT8_MAX, ((uint8_t *)&settings_in_modified)[i]);
    }
    ASSERT_TRUE(esp32.Write(ObjectDictionary::Address::kAddrSettingsStruct, settings_in_original));
}

UTEST(SPICoprocessor, ReadWriteReadRewriteRereadBigWithAck) {
    SettingsManager::Settings settings_in_original;
    memset(&settings_in_original, 0x0, sizeof(settings_in_original));
    ASSERT_TRUE(esp32.Read(ObjectDictionary::Address::kAddrSettingsStruct, settings_in_original));
    SettingsManager::Settings settings_out;
    for (uint16_t i = 0; i < sizeof(settings_out); i++) {
        ((uint8_t *)&settings_out)[i] = i % UINT8_MAX;
    }
    ASSERT_TRUE(esp32.Write(ObjectDictionary::Address::kAddrSettingsStruct, settings_out, true));
    SettingsManager::Settings settings_in_modified;
    ASSERT_TRUE(esp32.Read(ObjectDictionary::Address::kAddrSettingsStruct, settings_in_modified));
    for (uint16_t i = 0; i < sizeof(settings_out); i++) {
        EXPECT_EQ(i % UINT8_MAX, ((uint8_t *)&settings_in_modified)[i]);
    }
    ASSERT_TRUE(esp32.Write(ObjectDictionary::Address::kAddrSettingsStruct, settings_in_original, true));
}