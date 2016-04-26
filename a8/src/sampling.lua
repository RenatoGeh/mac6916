local Sampling = {}

Utils = require "utils"
Factor = require "factor"

-- Qr here is a distribution.
local function get_distribution(Qr, E)
  local Pr = {}
  local reduced = Qr

  -- Reduce the potential if necessary.
  if E then
    for _,e in E do
      reduced = reduced[e]
    end
  end

  for _,p in reduced do
    table.insert(Pr, p)
  end

  return Pr

  --[[if not E then return Qr end

  local n, m = #Qr, #(phi.Sc)
  local np = phi.Sc[1].n
  local T, Pr = {}, {}

  for i=2,m,1 do
    table.insert(T, (phi._v[i]*E[i]) + 1)
  end

  local v_first = phi._v[1]
  for i=1,np,1 do
    local p
    for k=2,m,1 do
      for j=T[i],phi.Sc[k].n,1 do

      end
    end
    table.insert(Pr, p)
  end--]]
end

--[[ Generates a sample from a probability distribution.
--
-- Pr : distribution (potential)
--
-- Returns a value of Pr.
--]]
function Sampling.generate(Pr)
  local sample, acc = math.random(), 0

  for k,p in Pr do
    if sample < acc + p and sample >= acc then
      return p
    end
    acc = acc + p
  end

  -- What?
  return nil
end

--[[ Approximate Inference by Logical Sampling
--
--  Let V the set of nodes of a Bayesian Network under topological order O, P a set of potentials,
--  E an instantiation of evidence and m the number of samples.
--
--  A set of nodes V that obeys order O has the property that every root node is at the beginning
--  of the array. Let n be the number of root nodes in Bayesian Network N, then there exists a
--  contiguous subsequence of V, U = V[1..n] such that it is an ordered set of root nodes, and
--  every V_i, where i>n, is a child node of a node in U.
--
--  Evidence E is formatted such that E[i] can either be a value of variable i if there exists an
--  evidence variable in E or nil if there is no evidence value of that variable.
--
--  Returns Pr(E=e)
--]]
function Sampling.logical(V, P, E, m)
  -- Set of samples.
  local S = {}
  local n = #V

  for i=1,m,1 do
    S[i] = {}
  end

  for i=1,n,1 do
    local v, p = V[i], P[i]
    local pr, s = p.Pr, nil -- s current sample vals
    local sc, vars = #(P.scope), {}
    if sc > 1 then
      -- Checks for dependencies (must be < i because of topological order).
      for j=1,i,1 do
        if Utils.contains(P.scope, V[j]) then
          table.insert(vars, j)
        end
      end
      local nvars = #vars

      -- Builds s according to dependencies.
      for j=1,m,1 do
        s[j] = {}
        for k=1,nvars,1 do
          table.insert(s[j], S[vars[k]])
        end
      end

      for j=1,m,1 do
        table.insert(S[j], Sampling.generate(get_distribution(P[i].map, s[j])))
      end
    else -- Root node
      for j=1,m,1 do
        table.insert(S[j], Sampling.generate(get_distribution(P[i].map, nil)))
      end
    end
  end

  -- Compute Av(x).
  local p = 0
  for i=1,m,1 do
    for j=1,n,1 do
      if E[i] and S[i][j] == E[i] then
        p = p + 1
      end
    end
  end

  return p/m
end

--[[ Likelihood weighting sampling
-- V is a set of variables in topological order.
-- P is a set of potentials such that P[i] is the potential of variable V[i].
-- E is a set of evidence where E[i] is nil iff there is no evidence for V[i]; otherwise V[i]=E[i].
-- m is the number of sample iterations.
--]]
function Sampling.likelihood(V, P, E, m)
  local _P = {} -- likelihood-weighting CPTs.
  local pars = {} -- Evidence parameters.
  local n = #P

  -- Creates likelihood-weighting network.
  for i=1,n,1 do
    if E[i] then
      local phi = {}
      for j=1,V[i].n,1 do
        if E[i] == V[i].vals[j] then
          phi[j] = 1
        else
          phi[j] = 0
        end
      end
      table.insert(pars, i)
      _P[i] = Factor.create({V[i]}, phi)
    else
      _P[i] = P[i]
    end
  end

  local S = {}

  -- Generate samples from likelihood-weighting network.
  for i=1,n,1 do
    local v, p = V[i], _P[i]
    local pr, s = p.Pr, nil -- s current sample vals
    local sc, vars = #(_P.scope), {}
    if sc > 1 then
      -- Checks for dependencies (must be < i because of topological order).
      for j=1,i,1 do
        if Utils.contains(_P.scope, V[j]) then
          table.insert(vars, j)
        end
      end
      local nvars = #vars

      -- Builds s according to dependencies.
      for j=1,m,1 do
        s[j] = {}
        for k=1,nvars,1 do
          table.insert(s[j], S[vars[k]])
        end
      end

      for j=1,m,1 do
        table.insert(S[j], Sampling.generate(get_distribution(_P[i].map, s[j])))
      end
    else -- Root node
      for j=1,m,1 do
        table.insert(S[j], Sampling.generate(get_distribution(_P[i].map, nil)))
      end
    end
  end

  local Pr_e = 0 -- Probability of evidence Pr(E=e).
  local Pr_xe = {} -- Probability Pr(x,e) for each value x.

  for i,_ in pairs(V) do
    for j,_ in pairs(v) do
      Pr[i][j] = 0
    end
  end

  for _,x in ipairs(S) do
    local w = 1
    for i,i_par in ipairs(pars) do
      local pi = P[i_par]
      local reduced = pi.map
      local eu = {} -- Consistent vars.

      eu[1] = x[i_par]
      for j=2,#(pi.Sc),1 do
        for k=1,n,1 do
          if V[k] == pi.Sc[j] then
            eu[j] = k
            break
          end
        end
      end

      for j=1,#(pi.sc),1 do
        reduced = reduced[x[eu[j]]] -- Should be a number in the last iteration.
      end

      w = w * reduced
    end
    Pr_e = Pr_e + w

    for j,v in pairs(V) do
      for k,_ in pairs(v) do
        if x[j] == v[k] then
          Pr_xe[j][k] = Pr_xe[j][k] + w
        end
      end
    end
  end

  -- Normalize.
  for i,_ in pairs(Pr_xe) do
    for j,_ in pairs(Pr_xe[i]) do
      Pr_xe[i][j] = Pr_xe[i][j]/P
    end
  end

  return P/m, Pr_xe
end

return Sampling
