# include  <iverilog/ivl_target.h>
# include <stdio.h>
# include <stdlib.h>

#define DEBUG0(...) printf("\e[1;34m AIG TARGET DEBUG:\e[0m " __VA_ARGS__)
#define WARNING(...) printf("\e[0;33m AIG TARGET WARNING:\e[0m " __VA_ARGS__)


int process_scope(ivl_scope_t * scope);
static int show_process(ivl_process_t net, void * x);
int show_constants(ivl_design_t des);

int target_design(ivl_design_t des)
{
  ivl_scope_t  ** scopes;
  scopes = malloc(sizeof(ivl_scope_t *));
  ivl_scope_t scope;
  int num_scopes;
  const char * bleh;
  DEBUG0("Design targeted for AIG\n");

  ivl_design_roots(des,scopes,&num_scopes);
  DEBUG0("Processing %d scopes\n",num_scopes);

  int i = 0;
  while (i < num_scopes){
    process_scope(*scopes++);
    i++;
  }

  ivl_design_process(des,show_process,0);

  show_constants(des);

  return 0;
}

int process_scope(ivl_scope_t * scope){
  DEBUG0("Scope name: %s\n",ivl_scope_name(*scope));
  return 0;
}
/*
      IVL_ST_NONE    = 0,
      IVL_ST_NOOP    = 1,
      IVL_ST_ALLOC   = 25,
      IVL_ST_ASSIGN    = 2,
      IVL_ST_ASSIGN_NB = 3,
      IVL_ST_BLOCK   = 4,
      IVL_ST_CASE    = 5,
      IVL_ST_CASER   = 24, // Case statement with real expressions.
      IVL_ST_CASEX   = 6,
      IVL_ST_CASEZ   = 7,
      IVL_ST_CASSIGN = 8,
      IVL_ST_CONDIT  = 9,
      IVL_ST_CONTRIB = 27,
      IVL_ST_DEASSIGN = 10,
      IVL_ST_DELAY   = 11,
      IVL_ST_DELAYX  = 12,
      IVL_ST_DISABLE = 13,
      IVL_ST_FORCE   = 14,
      IVL_ST_FOREVER = 15,
      IVL_ST_FORK    = 16,
      IVL_ST_FREE    = 26,
      IVL_ST_RELEASE = 17,
      IVL_ST_REPEAT  = 18,
      IVL_ST_STASK   = 19,
      IVL_ST_TRIGGER = 20,
      IVL_ST_UTASK   = 21,
      IVL_ST_WAIT    = 22,
      IVL_ST_WHILE   = 23
*/
static int show_process(ivl_process_t net, void * x){
  DEBUG0("Process type %d from %s on line %d\n",ivl_statement_type(ivl_process_stmt(net)),ivl_process_file(net),ivl_process_lineno(net));

  switch(ivl_statement_type(ivl_process_stmt(net))) {
    case IVL_ST_ASSIGN:
      DEBUG0("Assign Statement\n");
      break;
    case IVL_ST_ASSIGN_NB:
      DEBUG0("Non-blocking assign statement\n");
      break;
    default:
      WARNING("Unsupported statement\n");
  }
  ivl_scope_t scope = ivl_process_scope(net);
  process_scope(&scope);

  return 0;
}

int show_constants(ivl_design_t des){
  unsigned i;

  for(i=0;i<ivl_design_consts(des);i++){
    ivl_net_const_t net_const = ivl_design_const(des,i);
    //ivl_variable_type_t var_type = ivl_const_type(net_const);
    DEBUG0("%d\tDesign Constant type \n",i);
  }
  return 0;
}


