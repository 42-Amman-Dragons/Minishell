6. generate_expanded_list (bonus)--> `new_args = ft_calloc(args_len + cal_len(expanded, ' ') + 1, sizeof(char *));`
6. generate_expanded_list--> `new_args = ft_calloc(args_len + cal_len(expanded, ' ') + 1, sizeof(char *));` 
7. generate_expanded_list_asterisk (bonus) --> `new_args = ft_calloc(args_len + expanded_len + 1, sizeof(char *));` invalid free.



# Checked

1. append_string_to_list  --> `new_list = malloc(sizeof(char *) * (size + 2));`
3. handle_normal --> if any of the result fails the programs stops reciving inputs and does not close (maby it enters an infinite loop or deadlock).
4. process_entry --> `*result = append_string_to_list(result, ft_strdup(cf->d_name));` strdup failer resulted in leaks.

5. handle_single_match --> `*expanded = ft_strdup(matched_paths[0]);`

# Undecided
2. cd_get_path --> path_out malloc failiers.