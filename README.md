# gerenciamentoDeProcessos
Implementação de um interpretador de comandos e um escalonador de processos

O interpretador deverá solicitar ao escalonador a execução de programas. O escalonador por sua vez dispara a execução dos programas de acordo com políticas de escalonamento, que podem ser:

    PRIORIDADE (neste caso o interpretador deve indicar ao escalonador uma prioridade de execução, que é um número de 1 a 7 sendo 1 a maior prioridade e 7 a menor) .
    ROUND-ROBIN – processos CPU-bound (são os processos de prioridade 7).
    ROUND-ROBIN – processos I/O-bound (são os processos de prioridade 6).
    REAL TIME (são os processos de prioridade 1 ou 2).

O escalonador deve ter estruturas de dados capazes de possibilitar a execução destas políticas. Você deve implementar um único escalonador para todas as políticas. A comunicação entre os processos é feita da seguinte forma:

Interpretador de comandos -> Escalonador -> Sistema Operacional

A linguagem a ser analisada pelo interpretador de comandos é a seguinte:

    exec <nome_programa> prioridade=<número de 1 a 7>

Os programas a serem escalonados/ controlados devem ser de dois tipos: CPU-bound e I/Obound e devem ser criados por vocês (por exemplo, processos CPU-bound podem ter loops eternos e processos I/O-bound podem também ter loops eternos, mas com etapas de execução que duram 2 segundos e são interrompidas a cada segundo para executar uma operação de I/O que dura 3 segundos.

O interpretador irá ler de exec.txt quais são os programas a serem executados, e deverá inicia-los exatamente na ordem em que aparecem nesse arquivo, com um intervalo de 1 segundo entre cada um deles. Os programas são passados um-a-um para o escalonador. A saída deve ser gerada no arquivo saída.txt e deve clara o suficiente para mostrar como e porque ocorre a preempção na execução dos processos. 
