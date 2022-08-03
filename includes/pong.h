/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RTv1.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngardzie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 11:43:26 by ngardzie          #+#    #+#             */
/*   Updated: 2022/06/22 11:43:29 by ngardzie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RTV1_H
# define RTV1_H

# define WIDTH 640
# define HEIGHT 480
# define PLAYER_WIDTH 20
# define PLAYER_HEIGHT 70
# define PLAYER_MARGIN 20


typedef struct s_Ball {
    float x;
    float y;
    float xSpeed;
    float ySpeed;
    int size;
} t_Ball;

typedef struct s_Player {
    int score;
    float yPosition;
} t_Player;

# include <math.h>
# include <fcntl.h>

#endif
