#include <windows.h>
#include <stdio.h>

typedef long NTSTATUS;
#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)

typedef struct _SYSTEM_PROCESSOR_INFORMATION {
    ULONG ProcessorArchitecture;
    ULONG ProcessorLevel;
    ULONG ProcessorRevision;
    ULONG NumberOfProcessors; 
    ULONG ActiveProcessorsAffinityMask;
} SYSTEM_PROCESSOR_INFORMATION;

typedef enum _SYSTEM_INFORMATION_CLASS {
    SystemProcessorInformation = 1  // Classe de informações sobre os processadores
} SYSTEM_INFORMATION_CLASS;

typedef NTSTATUS(WINAPI* pNtQuerySystemInformation)(
    SYSTEM_INFORMATION_CLASS SystemInformationClass,
    PVOID SystemInformation,
    ULONG SystemInformationLength,
    PULONG ReturnLength
);

volatile BOOL executarCodigoKernel = FALSE;

DWORD WINAPI TarefaModoUsuario(LPVOID lpParam) {
    while (1) {
        if (executarCodigoKernel) {
            printf("Modo usuário: A função do modo kernel será chamada agora.\n");
            executarCodigoKernel = FALSE;

            // Chama a função do modo kernel (via NtQuerySystemInformation)
            HMODULE hNtDll = GetModuleHandleW(L"ntdll.dll");
            if (hNtDll == NULL) {
                printf("Erro: Não foi possível carregar o ntdll.dll\n");
                return 1;
            }

            pNtQuerySystemInformation NtQuerySystemInformation = (pNtQuerySystemInformation)GetProcAddress(hNtDll, "NtQuerySystemInformation");
            if (NtQuerySystemInformation == NULL) {
                printf("Erro: Não foi possível encontrar NtQuerySystemInformation\n");
                return 1;
            }

            // Consulta as informações sobre os processadores
            ULONG returnLength;
            SYSTEM_PROCESSOR_INFORMATION spi;
            NTSTATUS status = NtQuerySystemInformation(SystemProcessorInformation, &spi, sizeof(spi), &returnLength);

            if (status == 0) {
                printf("Modo kernel: Informações sobre os processadores recuperadas com sucesso\n");
                printf("Número de processadores: %lu\n", spi.NumberOfProcessors);
                printf("Máscara de Processadores Ativos: 0x%lx\n", spi.ActiveProcessorsAffinityMask);
            } else {
                printf("Erro: Não foi possível consultar as informações do sistema, código de status: 0x%lx\n", status);
                if (status == 0xc0000004) {
                    printf("STATUS_INVALID_INFO_CLASS: A classe de informações fornecida não é válida.\n");
                }
            }
        }

        Sleep(100); // Dorme por um tempo para evitar alto uso de CPU
    }
    return 0;
}

int main() {
    HANDLE hThreadUsuario = CreateThread(NULL, 0, TarefaModoUsuario, NULL, 0, NULL);
    if (hThreadUsuario == NULL) {
        printf("Erro ao criar a thread do modo usuário.\n");
        return 1;
    }

    while (1) {
        char entrada;
        printf("Pressione 'k' para invocar uma função do modo kernel, ou 'q' para sair: ");
        entrada = getchar();
        getchar(); // Consome o caractere de nova linha

        if (entrada == 'k') {
            executarCodigoKernel = TRUE;  
            printf("Modo usuário: Solicitando a execução de função do modo kernel...\n");
        } else if (entrada == 'q') {
            break;  
        }
    }

    WaitForSingleObject(hThreadUsuario, INFINITE);  // Aguarda a thread do modo usuário terminar
    CloseHandle(hThreadUsuario);

    return 0;
}
