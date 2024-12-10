#include <iostream>
#include <string>
#include <vector>
#include <locale>
#ifdef _WIN32
#include <windows.h>
#include <conio.h> // Para usar _getch()
#endif

class Conta {
private:
    int numeroConta;
    int senha;
    double saldo;

public:
    Conta(int numero, int senha, double saldoInicial)
        : numeroConta(numero), senha(senha), saldo(saldoInicial) {}

    int getNumeroConta() const { return numeroConta; }
    bool autenticar(int senhaInput) const { return senha == senhaInput; }

    double getSaldo() const { return saldo; }

    bool sacar(double quantia) {
        if (quantia > saldo) {
            return false; // Indica falha no saque
        }
        saldo -= quantia;
        std::cout << "Saque realizado com sucesso. Novo saldo: R$ " << saldo << "\n";
        return true; // Indica sucesso no saque
    }

    void depositar(double quantia) {
        saldo += quantia;
        std::cout << "Depósito realizado com sucesso. Novo saldo: R$ " << saldo << "\n";
    }
};

class Banco {
private:
    std::vector<Conta> contas;

public:
    void adicionarConta(int numero, int senha, double saldoInicial) {
        contas.emplace_back(numero, senha, saldoInicial);
    }

    Conta* autenticarConta(int numero, int senha) {
        for (auto& conta : contas) {
            if (conta.getNumeroConta() == numero && conta.autenticar(senha)) {
                return &conta;
            }
        }
        return nullptr;
    }
};

class CaixaEletronico {
private:
    Banco& banco;

    // Função para ler a senha com ocultação de caracteres
    void lerSenha(int& senha) {
        senha = 0;
        char ch;
        while ((ch = _getch()) != '\r') { // Espera o enter para terminar a senha
            if (ch == 8) { // Backspace
                if (senha > 0) {
                    senha /= 10; // Remove o último dígito
                    std::cout << "\b \b"; // Apaga o caractere na tela
                }
            } else if (ch >= '0' && ch <= '9') {
                senha = senha * 10 + (ch - '0');
                std::cout << "*"; // Exibe o asterisco no lugar do número
            }
        }
        std::cout << std::endl; // Nova linha após a senha
    }

public:
    CaixaEletronico(Banco& b) : banco(b) {}

    void iniciar() {
        std::cout << "Bem-vindo ao Sistema de Caixa Eletrônico!\n";
        int numero, senha;
        std::cout << "Por favor, digite o número da sua conta: ";
        std::cin >> numero;
        std::cout << "Digite a sua senha: ";
        lerSenha(senha); // Usamos a função para ler a senha

        Conta* conta = banco.autenticarConta(numero, senha);
        if (!conta) {
            std::cout << "Número da conta ou senha incorretos.\n";
            return;
        }

        bool sair = false;
        while (!sair) {
            std::cout << "\nMenu:\n";
            std::cout << "1. Ver saldo\n";
            std::cout << "2. Sacar dinheiro\n";
            std::cout << "3. Depositar dinheiro\n";
            std::cout << "4. Sair\n";
            std::cout << "Escolha uma opção: ";
            int opcao;
            std::cin >> opcao;

            switch (opcao) {
            case 1:
                std::cout << "Seu saldo atual é: R$ " << conta->getSaldo() << "\n";
                break;
            case 2: {
                double quantia;
                std::cout << "Digite o valor para sacar: ";
                std::cin >> quantia;
                if (!conta->sacar(quantia)) {
                    std::cout << "Opção inválida. Saldo insuficiente para o saque.\n";
                }
                break;
            }
            case 3: {
                double quantia;
                std::cout << "Digite o valor para depositar: ";
                std::cin >> quantia;
                conta->depositar(quantia);
                break;
            }
            case 4:
                std::cout << "Obrigado por utilizar o Caixa Eletrônico. Até logo!\n";
                sair = true;
                break;
            default:
                std::cout << "Opção inválida.\n";
            }
        }
    }
};

int main() {
    // Configuração para UTF-8
    setlocale(LC_ALL, ""); // Habilitar suporte à localização
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    #endif

    Banco banco;
    banco.adicionarConta(12345, 1234, 3000.0); // Exemplo de conta para teste

    CaixaEletronico caixa(banco);
    caixa.iniciar();

    return 0;
}
