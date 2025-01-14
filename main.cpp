/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrichard <mrichard@student.42porto.pt>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 19:40:11 by mrichard          #+#    #+#             */
/*   Updated: 2025/01/14 20:00:14 by mrichard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <stdlib.h>

using std::cout;
using std::endl;

int main(int argc, char **argv)
{
    (void)argv;
    if (argc != 3)
    {
        cout << "Invalid number of params!" << endl;
        return 1;
    }
}
