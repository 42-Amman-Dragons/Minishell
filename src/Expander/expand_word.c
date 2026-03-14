/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_word.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuqare  <mabuqare@student.42amman.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 04:00:00 by mabuqare          #+#    #+#             */
/*   Updated: 2026/03/13 00:23:32 by mabuqare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_normal(char *word, t_expand *ctx)
{
	if (word[ctx->i] == '\'')
	{
		ctx->state = EXP_SQUOTE;
		ctx->i++;
	}
	else if (word[ctx->i] == '"')
	{
		ctx->state = EXP_DQUOTE;
		ctx->i++;
	}
	else if (word[ctx->i] == '$' && word[ctx->i + 1] && word[ctx->i + 1] != ' ')
		ctx->result = append_str(ctx->result, expand_dollar(word, ctx));
	else
		ctx->result = append_char(ctx->result, word[ctx->i++]);
}

static void	handle_squote(char *word, t_expand *ctx)
{
	if (word[ctx->i] == '\'')
	{
		ctx->state = EXP_NORMAL;
		ctx->i++;
	}
	else
		ctx->result = append_char(ctx->result, word[ctx->i++]);
}

static void	handle_dquote(char *word, t_expand *ctx)
{
	if (word[ctx->i] == '"')
	{
		ctx->state = EXP_NORMAL;
		ctx->i++;
	}
	else if (word[ctx->i] == '$' && word[ctx->i + 1] && word[ctx->i + 1] != '"')
		ctx->result = append_str(ctx->result, expand_dollar(word, ctx));
	else
		ctx->result = append_char(ctx->result, word[ctx->i++]);
}

char	*expand_word(char *word, char **env, int exit_status)
{
	t_expand	ctx;

	ctx.result = NULL;
	ctx.env = env;
	ctx.i = 0;
	ctx.exit_status = exit_status;
	ctx.state = EXP_NORMAL;
	while (word[ctx.i])
	{
		if (ctx.state == EXP_NORMAL)
			handle_normal(word, &ctx);
		else if (ctx.state == EXP_SQUOTE)
			handle_squote(word, &ctx);
		else if (ctx.state == EXP_DQUOTE)
			handle_dquote(word, &ctx);
	}
	if (!ctx.result)
		return (ft_strdup(""));
	return (ctx.result);
}
