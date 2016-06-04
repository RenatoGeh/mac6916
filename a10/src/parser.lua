local Parser = {}

Variable = require "variable"
Bayes = require "bayes"

function Parser.read()
  local type = io.read("*l") -- MARKOV or BAYES.
  local n = io.read("*n") -- Number of variables.
  local vals = {} -- Cardinalities of each variable.

  for i=1,n,1 do
    vals[i] = io.read("*n")
  end

  local vars = {}
  for i=1,n,1 do
    vars[i] = Variable.create(vals[i])
  end

  local np = io.read("*n") -- Number of potentials.
  local P = {} -- Set of potentials.
  local Sc = {} -- Set of scopes.

  for i=1,np,1 do
    Sc[i] = {}
    local n_sc = io.read("*n") -- Number of variables in potential.
    for j=1,n_sc,1 do
      Sc[i][j] = vars[io.read("*n") + 1] -- Indexed from 1.
    end
  end

  -- NOTE: If Bayes then the node variable must come last!
  for i=1,np,1 do
    local entries = io.read("*n")
    local Pr = {}

    for j=1,entries,1 do
      Pr[j] = io.read("*n")
    end

    P[i] = Factor.create(Sc[i], Pr)
  end

  if type == "BAYES" then
    return Bayes.create(vars, P)
  end

  return vars, P
end

return Parser
