# Sistemas Computacionais Embarcados - UFSCar

Coleção de projetos desenvolvidos para a disciplina de Sistemas Computacionais Embarcados da Universidade Federal de São Carlos (UFSCar). Todos os projetos são implementados para microcontroladores da família 8051.

## Projetos

### 🎮 [Campo Minado](./campo-minado/)
Implementação do clássico jogo Campo Minado com LCD 16x2 e teclado matricial.
- **Hardware**: 8051, LCD 16x2, teclado matricial 4x3
- **Características**: Campo 2x16, geração aleatória com LFSR, interface intuitiva

### 🦕 [Chrome Dino Game](./chrome-dino/)
Versão do famoso jogo do dinossauro do Google Chrome para sistemas embarcados.
- **Hardware**: 8051, LCD 16x2, teclado matricial
- **Características**: Personagem animado, obstáculos aleatórios, sistema de pontuação

### 🔒 [Sistema de Senha para Cofre](./senha-cofre/)
Sistema de autenticação por senha simulando segurança de cofre.
- **Hardware**: 8051, LCD 16x2, teclado matricial
- **Características**: Entrada segura com asteriscos, validação de senha, interface clara

### 🏎️ [Stock Car](./stock-car/)
Jogo de corrida estilo arcade com display LCD gráfico.
- **Hardware**: 8051, LCD gráfico 128x64, botões
- **Características**: Gráficos bitmap, sistema de vidas, controle de velocidade

## Ferramentas de Desenvolvimento

- **IDE**: Keil µVision
- **Compilador**: Keil C51
- **Simulação**: Proteus (recomendado)
- **Programação**: Programadores compatíveis com 8051

## Estrutura dos Projetos

Cada projeto contém:
```
projeto/
├── README.md        # Documentação específica
├── *.c              # Código fonte C
├── *.h              # Headers
├── *.uvproj         # Projeto Keil (quando aplicável)
├── Listings/        # Arquivos de listagem
└── Objects/         # Objetos compilados e hex files
```

## Características Técnicas Gerais

### Microcontrolador 8051
- **Arquitetura**: Harvard, 8 bits
- **Memória**: RAM interna limitada, uso eficiente necessário
- **Periféricos**: Timers, UART, portas digitais
- **Clock**: Varia conforme implementação (tipicamente 11.0592 MHz)

### Displays LCD
- **16x2**: Interface paralela 4-bit para economia de pinos
- **128x64**: Interface paralela com controladores KS0108
- **Caracteres customizados**: Definição de sprites quando necessário

### Teclados Matriciais
- **Configuração**: 4 linhas × 3 colunas (típico)
- **Debounce**: Implementado em software
- **Mapeamento**: Configurável conforme necessidade

---

**Universidade Federal de São Carlos (UFSCar)**  
**Disciplina**: Sistemas Computacionais Embarcados  
**Autor**: Vitor Diniz
**Professor**: Emerson Carlos Pedrino
