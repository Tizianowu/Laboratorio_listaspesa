//
// Created by Tiziano Wu on 01/09/24.
//
#include "gtest/gtest.h"
#include "../Mainframe.h"

TEST(ItemTest, ConstructorInitializesMembers) {
    // Definisci i valori di test
    wxString testName = "TestItem";
    int testQuantity = 10;

    // Crea un oggetto Item utilizzando il costruttore
    Item item(testName, testQuantity);

    // Verifica che i membri siano stati inizializzati correttamente
    EXPECT_EQ(item.getName(), testName);
    EXPECT_EQ(item.getQuantity(), testQuantity);
}
