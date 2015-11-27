#####################################################SET'S###################################################################

set POLOS_ABASTECIMENTO_TERMINAIS ordered;	#polos de demanda que foram selecionados para terminal na fase I
set POLOS_SUPRIMENTO ordered;	#polos de suprimentos que foram selecionados para terminal na fase I
set POLOS_SUPRIMENTO_TERMINAIS within POLOS_SUPRIMENTO ordered;

set POLOS = POLOS_ABASTECIMENTO_TERMINAIS union POLOS_SUPRIMENTO;
set TERMINAIS = POLOS_ABASTECIMENTO_TERMINAIS union POLOS_SUPRIMENTO_TERMINAIS;
set CANAIS = {i in TERMINAIS, j in TERMINAIS: i != j} union {i in (POLOS_SUPRIMENTO diff POLOS_SUPRIMENTO_TERMINAIS), j in TERMINAIS: i != j};

#####################################################PARAMETROS#############################################################

param fluxo_max{CANAIS};
param demanda{POLOS_ABASTECIMENTO_TERMINAIS union POLOS_SUPRIMENTO};

param custo_fluxo{CANAIS};
param custo_abertura{CANAIS};

#####################################################VARIAVEIS###############################################################

var aberto{CANAIS} binary;
var fluxo{CANAIS} >= 0;

#####################################################OBJETIVA################################################################

minimize CUSTO_GERAL:
	sum{(i, j) in CANAIS} aberto[i, j]*custo_abertura[i, j] +
	sum{(i, j) in CANAIS} fluxo[i, j]*custo_fluxo[i, j];

s.t. DEMANDA{i in POLOS}:
	sum{(j, i) in CANAIS} fluxo[j, i] -
	sum{(i, j) in CANAIS} fluxo[i, j] >= demanda[i];

s.t. CONTROLE_FLUXO{(i, j) in CANAIS}:
	fluxo[i, j] <= aberto[i, j]*fluxo_max[i, j];
