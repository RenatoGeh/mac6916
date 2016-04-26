local Message = {}

Factor = require "factor"

-- Creates a factor graph from set of potentials P and scope S.
function Message.factor_graph(P, S)
  local adj_list, n_P, n_S = {}, #P, #S
  local n = n_P + n_S

  -- Creates nodes.
  for _,phi in ipairs(P) do
    adj_list[phi] = {}
  end
  for _,var in ipairs(S) do
    adj_list[var] = {}
  end

  -- Creates edges.
  for _,phi in ipairs(P) do
    local ne, sc = adj_list[i], phi.Sc
    for _,v in ipairs(sc) do
      adj_list[phi][v] = {function (X)
        -- Message passing function for:
        -- Local function (factor) to variable.
        adj_list[phi][v][2] = phi

        for _,avar in pairs(adj_list[phi]) do
          if avar ~= X then
            adj_list[phi][v][2] = Factor.multiply({adj_list[phi][v][2],
              adj_list[phi][avar][1](avar)})
          end
        end

        for _,avar in ipairs(phi.Sc) do
          if avar ~= X then
            adj_list[phi][v][2] = Factor.sum(adj_list[phi][v][2], avar)
          end
        end

        return adj_list[phi][v][2]
      end, nil}

      adj_list[v][phi] = {function (X)
        -- Message passing function for:
        -- Variable to local function (factor).
        for _,aphi in pairs(adj_list[v]) do
          if aphi ~= phi then
            if adj_list[v][phi][2] then
              adj_list[v][phi][2] = Factor.multiply({adj_list[v][phi][2], adj_list[v][aphi][1](X)})
            else
              adj_list[v][phi][2] = adj_list[v][aphi][1](X)
            end
          end
        end

        return adj_list[v][phi][2]
      end, nil}
    end
  end
  return adj_list
end

-- Takes factor graph G and scope S. Computes marginals for every variable.
function Message.sp_factor(G, S)
  local marg = {}

  for _,v in ipairs(S) do
    local M = {}
    for _,f in pairs(adj_list[v]) do
      table.insert(M, adj_list[v][f][1](v))
    end
    marg[v] = Factor.multiply(M)
  end

  return marg
end

return Message
