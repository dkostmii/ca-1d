function concat(t1, t2)
  local result = {}

  for _, item in ipairs(t1) do
    table.insert(result, item)
  end

  for _, item in ipairs(t2) do
    table.insert(result, item)
  end

  return result
end

return {
  "goolord/alpha-nvim",
  dependencies = { "nvim-tree/nvim-web-devicons" },
  config = function()
    local startify_theme = require("alpha.themes.startify")

    local padding = {
      top = 2,
      left = 3,
    }

    local width = vim.fn.winwidth("%") - 2 * padding.left

    local caption = {
      "",
      "Welcome to NeoVIM!",
    }

    local art = concat(vim.fn.systemlist("python ~/welcome_art_new.py " .. width), caption)

    startify_theme.section.header.val = art
    startify_theme.section.header.opts.hl = nil

    local alpha = require("alpha")
    alpha.setup(startify_theme.config)

    vim.api.nvim_create_autocmd("User", {
      pattern = "AlphaReady",
      callback = function()
        vim.cmd([[syntax match WelcomeArt1 /[@K&%#*?$]/]])
        vim.cmd([[syntax match WelcomeArt2 /[MW\>86RBE]/]])
        vim.cmd([[syntax match WelcomeArt3 /[\=3\~xdVN\<]/]])
        vim.cmd([[syntax match WelcomeArt4 /[:;\"\'`]/]])
        vim.cmd([[syntax match WelcomeArt5 /[qojsei]/]])
        vim.cmd([[syntax match WelcomeArt6 /[\/\.,\|_\-]/]])
        vim.cmd([[syntax match Title /Welcome to NeoVIM!/]])
        vim.cmd([[syntax match Normal /\(MRU.*\|\[.\+\].*\)/]])

        vim.cmd.file("alpha")
        alpha.redraw()
      end,
    })

    vim.keymap.set("n", "<leader>h", function()
      if string.lower(vim.bo.filetype) == "alpha" then
        local width = vim.fn.winwidth("%") - 2 * padding.left
        art = concat(vim.fn.systemlist("python ~/welcome_art_new.py " .. width), caption)

        startify_theme.section.header.val = art
        startify_theme.section.header.opts.hl = nil
        alpha.redraw()
      end
    end, { noremap = true })
  end,
}
