local InteractionGraph = {}

Factor = require "factor"

--[[ Exact Inference via Variable Elimination
--
-- Takes a Bayesian Network and computes a query through Variable Elimination.
--
--]]

-- Note to self: arrays indexed [1..n].

--[[ Creates an interaction graph from a data table.
-- data {
--   vars : table of variables
--   pots : table of potencials
-- }
--
-- The resulting interaction graph is represented as an adjacency list.
-- adj_list {
--   var_1 = { ... } : list of neighbours of var_1
--   var_2 = { ... } : list of neighbours of var_2
--   ...
--   var_n = { ... } : list of neighbours of var_n
-- }
--]]
function InteractionGraph.create_graph(data)
  -- Adjacency list for the interaction graph.
  local adj_list = {}

  -- Creates a list for each variable/node.
  local vars = data.vars
  for _,v in ipairs(vars) do
    adj_list[v] = {
      [1]=0      -- Number of hash key entries in adj_list (namely the number of neighbours).
    }
  end

  -- Assigns each node dependency.
  for _,p in ipairs(pots) do
    for _,sc1 in ipairs(pots.scope) do
      for _,sc2 in ipairs(pots.scope) do
        if sc1 ~= sc2 then
          adj_list[sc1][sc2], adj_list[sc2][sc1] = true, true
          adj_list[sc1][1], adj_list[sc2][1] = adj_list[sc1][1] + 1, adj_list[sc2][1] + 1
        end
      end
    end
  end

  return adj_list
end

--[[ Eliminates a variable v from the interaction graph G and set of potentials P.
--
-- Takes interaction graph G and set of potentials P and eliminates variable v from G,
-- multiplying and then summing out v in all potentials in P that have v in their scope.
--
-- Returns the potential pi, where pi is the resulting potential after multiplying and summing out
-- v from P.
--]]
function InteractionGraph.eliminate_var(v, G, P)
  -- Set of potentials that contain v in their scopes.
  -- This equals to ne_v. This is just a local copy.
  local phi = {}

  -- Constructs phi.
  for _,p in ipairs(P) do
    for _,u in ipairs(p.scope) do
      if u == v then
        table.insert(phi, p)
      end
    end
  end

  -- Neighbours of node v.
  local ne_v = G[v]
  -- Resulting potential after multiplying and summing out v.
  local pi = {}

  -- Transforms ne_v into a clique (multiply all factors).
  for u1,_ in pairs(ne_v) do
    if type(u1) ~= "number" then
      for u2,_ in pairs(ne_v) do
        if type(u2) ~= "number" and u1 ~= u2 and not G[u1][u2] then
          G[u1][u2], G[u2][u1] = true, true
          G[u1][1], G[u2][1] = G[u1][1] + 1, G[u2][1] + 1
        end
      end
    end
  end
  pi = Factor.multiply(phi)

  -- Removes node v and associated edges (sum out v).
  for u in pairs(ne_v) do
    if type(u) ~= "number" then
      G[u][v], G[v][u] = nil, nil
      G[u][1] = G[u][1] - 1
    end
  end
  G[v] = nil
  pi = Factor.sum(pi, v)

  -- Array of potentials to be removed.
  local rem = {}
  -- Replaces all potentials in phi with pi.
  for i,p in ipairs(P) do
    for _,q in ipairs(phi) do
      if p == q then
        table.insert(rem, i)
      end
    end
  end
  for _,k in ipairs(P) do
    table.remove(P, k)
  end
  table.insert(P, pi)

  return pi
end

--[[ Variable Elimination with Min-degree Heuristic.
--
-- Takes scope S, interaction graph G and set of potentials P.
-- For i=1..n, where n is the number of variables in G:
--   Let X be the variable where Ne(X) is minimum.
--   Eliminate variable X.
--
-- Returns the policy pi that represents the order of elimination of each variable and the
-- resulting potential phi.
--]]
function InteractionGraph.min_degree(S, G, P)
  -- Number of variables in S.
  local s = #S
  -- Policy pi.
  local pi = {}

  local phi, avar

  for i=1,s,1 do
    local min = s
    for _,v in ipairs(S) do
      local cs = G[v][1]
      if cs < min then
        avar, min = v, cs
      end
    end
    pi[i] = avar
    phi = eliminate_var(pi[i], G, P)
  end

  return pi, phi
end

--[[ Variable Elimination with Min-fill Heuristic.
-- Takes scope S, interaction graph G and set of potentials P.
-- For i=1..n, where n is the number of variables in G:
--   Let X be the variable where the additions of edges is minimum.
--   Eliminate variable X.
--
-- Returns policy pi that represents the order of elimination of each variable and the resulting
-- potential phi.
--]]
function InteractionGraph.min_fill(S, G, P)
  -- Number of variables in S.
  local s = #S
  -- Policy pi.
  local pi = {}

  local phi

  for i=1,s,1 do
    local min, avar = s, nil
    for _,x in ipairs(S) do
      local c, Ne = 0, G[x]
      for u,_ in pairs(Ne) do
        if type(u) ~= "number" then
          for v,_ in pairs(Ne) do
            if type(v) ~= "number" and u ~= v and not G[u][v] then
              c = c + 1
            end
          end
        end
        -- There can be at most two connections between two nodes when forming a clique.
        c = c / 2
      end
      if c < min then
        min, avar = c, x
      end
    end
    pi[i] = avar
    phi = eliminate_var(pi[i], G, P) -- Phi eventually has the value of the desired query.
  end

  return pi, phi
end

return InteractionGraph
