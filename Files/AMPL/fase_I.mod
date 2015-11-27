##############################____CONJUNTOS____#############################
set POLO_ABASTECIMENTO ordered;
set POLO_SUPRIMENTO ordered;
set TERMINAL_CANDIDATO within (POLO_ABASTECIMENTO union POLO_SUPRIMENTO);

##############################____PARAMETROS____############################

param custo_implantacao_terminal {TERMINAL_CANDIDATO} >= 0;
param demanda_polo {POLO_ABASTECIMENTO} >= 0;
param distancia_polo_terminal {TERMINAL_CANDIDATO, POLO_ABASTECIMENTO} >= 0;
param custo_transporte {TERMINAL_CANDIDATO, POLO_ABASTECIMENTO} >= 0;

##############################____VARIAVEIS____#############################

var esta_aberto {TERMINAL_CANDIDATO}, binary;
var fracao_demanda {TERMINAL_CANDIDATO, POLO_ABASTECIMENTO} >= 0, <= 1;

########################____FUNCAO_E_RESTRICOES____#########################

minimize custo_implantacao_transporte:
	sum {i in TERMINAL_CANDIDATO} (custo_implantacao_terminal[i]*esta_aberto[i]) +
	sum {i in TERMINAL_CANDIDATO, j in POLO_ABASTECIMENTO} (demanda_polo[j]*custo_transporte[i, j]*distancia_polo_terminal[i, j]*fracao_demanda[i, j]);

s.t. total_fracao {j in POLO_ABASTECIMENTO}:
	sum {i in TERMINAL_CANDIDATO} (fracao_demanda[i, j]) = 1;

s.t. limiteSuperior_fracao {i in TERMINAL_CANDIDATO, j in POLO_ABASTECIMENTO}:
	fracao_demanda[i, j] <= esta_aberto[i];
