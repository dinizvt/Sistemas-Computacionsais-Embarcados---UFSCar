# Sistema de Senha para Cofre

Sistema de autenticação por senha implementado para microcontrolador 8051 com display LCD 16x2 e teclado matricial, simulando um sistema de segurança para cofre.

## Descrição

Sistema que solicita uma senha de 4 dígitos ao usuário através de um teclado matricial e verifica se a senha está correta. O sistema fornece feedback visual através de um display LCD, ocultando os dígitos digitados com asteriscos para segurança.

## Hardware Necessário

- **Microcontrolador**: 8051
- **Display**: LCD 16x2 (modo 4 bits)
- **Teclado**: Matricial para entrada de dados
- **Interface**: RS232 para comunicação serial (opcional)

## Funcionalidades

- **Entrada segura**: Senha oculta com asteriscos
- **Verificação**: Comparação com senha predefinida
- **Feedback visual**: Mensagens de sucesso/erro no LCD
- **Interface intuitiva**: Prompts claros para o usuário

## Como Usar

1. **Inicialização**: O sistema exibe "Insira a senha:"
2. **Entrada**: Digite 4 dígitos no teclado matricial
3. **Visualização**: Cada dígito aparece como "*" no LCD
4. **Verificação**: Sistema compara com senha armazenada
5. **Resultado**: Exibe "Acesso liberado!" ou "Senha incorreta!"

## Configuração Padrão

- **Senha padrão**: `1234`
- **Tamanho**: 4 dígitos
- **Modificação**: Altere a constante `SENHA` no código para mudar a senha

## Arquivos do Projeto

```
senha-cofre/
├── senha.c          # Programa principal do sistema
├── lcd.c           # Driver do LCD
├── lcd.h           # Interface do LCD
├── keypad.h        # Interface do teclado
├── keymap.c        # Mapeamento do teclado
├── shared.c        # Funções utilitárias
├── shared.h        # Definições compartilhadas
├── serial.c        # Comunicação serial (opcional)
├── serial.uvproj   # Projeto Keil µVision
├── serial.uvopt    # Opções do projeto
├── STARTUP.A51     # Código de inicialização
├── Listings/       # Arquivos de listagem
└── Objects/        # Objetos compilados e executável
```

## Características de Segurança

- **Ocultação**: Dígitos mostrados como asteriscos
- **Validação completa**: Verifica todos os 4 dígitos
- **Senha fixa**: Armazenada em memória de programa
- **Interface clara**: Mensagens informativas

## Interface do Sistema

### Tela de Entrada
```
Insira a senha:
****
```

### Sucesso
```
Acesso liberado!
```

### Falha
```
Senha incorreta!
```

## Pinagem

### LCD
- Barramento de dados: P1[7:4] (modo 4 bits)
- RS: P1.3
- EN: P1.2

### Teclado Matricial
- Linhas: P0[3:0]
- Colunas: P0[6:4]

## Modificações Possíveis

1. **Alterar senha**: Modifique a constante `SENHA` no código
2. **Tamanho da senha**: Ajuste o tamanho do array e loops
3. **Múltiplas tentativas**: Adicione contador de tentativas
4. **Timeout**: Implemente timeout de segurança
5. **Log de tentativas**: Use interface serial para registro

## Estrutura do Código

### Função Principal
- Inicialização do LCD
- Loop de captura de 4 dígitos
- Verificação da senha
- Exibição do resultado

### Verificação de Senha
A função `checkPasswd()` compara caractere por caractere a entrada do usuário com a senha armazenada, retornando 1 para sucesso e 0 para falha.
